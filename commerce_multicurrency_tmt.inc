<?php

/**
 * Fetch the currency exchange rates for the requested currency combination.
 * 
 * With staging we use
 * https://trustmytravel.com/currency-data/
 * 
 * With live we use
 * https://staging.trustmytravel.com/currency-data/
 *
 * Return an array with the array(target_currency_code => rate) combination.
 *
 * @param string $currency_code
 *   Source currency code.
 * @param array $target_currencies
 *   Array with the target currency codes.
 *
 * @return array
 *   Array with the array(target_currency_code => rate) combination.
 */

function commerce_multicurrency_tmt_exchange_rate_sync_provider_tmt($currency_code, $target_currencies) {

  $data = cache_get(__FUNCTION__, 'cache');
  
  if (!$data) {
    
    // Set the URL to download from
    if (variable_get('commerce_multicurrency_tmt_feed', 'live') == 'live') {
      $url = "https://trustmytravel.com/currency-data/";
    } else {
      $url = "https://staging.trustmytravel.com/currency-data/";
    }
    
    // Download the data
    $json = file_get_contents($url);
    $json_data = json_decode($json, true);  

    if ($json_data) {
      // Calculate when to expire the cache (new TMT exchange rates are released each day at 12.30 GMT)
      $datetime = new DateTime('now', timezone_open('GMT')); // GMT as TMT API uses GMT
      if ($datetime->format('HH MM') > 1230) {
        $cachelimit = new DateTime('tomorrow', timezone_open('GMT'));
      } else {
        $cachelimit = new DateTime('today', timezone_open('GMT'));
      }
      $cachelimit->modify('+12 hours');
      $cachelimit->modify('+30 minutes');

      // Set the cache
      cache_set(__FUNCTION__, $json_data, 'cache', $cachelimit->getTimestamp());
      
      // Make $json_data available outside local scope
      $data['data'] = $json_data;
    } 
    else {
      watchdog(
        'commerce_multicurrency_tmt', 'Rate provider TMT: Unable to fetch / process the currency data of @url',
        array('@url' => $url),
        WATCHDOG_ERROR
      );
    }
  } 
  else {
    $json_data = $data->data;
  }
  
  $rates = array();
  foreach ($target_currencies as $target_currency_code) {
    if ((isset($json_data[$currency_code]['rates'][$target_currency_code])) 
            && ($json_data[$currency_code]['rates'][$target_currency_code] > 0) ) {
      $rates[$target_currency_code] = $json_data[$currency_code]['rates'][$target_currency_code]
              * (1 + (variable_get('commerce_multicurrency_tmt_markup_amount', 10)/100));
    }
    else {
      $rates[$target_currency_code] = 1.2;
    }
  }

  return $rates;
  
}
