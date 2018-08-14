<?php

/**
 * @file
 * Administrative UI for commerce multicurrency tmt.
 */



/**
 * Form to configure Trust My Travel currency API settings
 */
function commerce_multicurrency_tmt_settings_form($form, &$form_state) {
    
  $form['feed'] = array(
    '#type' => 'select',
     '#title' => t('Feed to use'),
     '#options' => array(
        'staging' => t('Staging'),
        'live' => t('Live'),
     ),
     '#default_value' => variable_get('commerce_multicurrency_tmt_feed', 'live'),
     '#description' => t('Set this to staging if you are using the staging API as well.'),
  );
  
  $form['markup_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Markup margin') . ' (%)',
    '#description' => t('Applied on top of the exchange rate.'),
    '#size' => 5,
    '#default_value' => variable_get('commerce_multicurrency_tmt_markup_amount', 10),
  );
      
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;
  
}

/**
 * Submit handler for the Trust My Travel currency API settings
 */
function commerce_multicurrency_tmt_settings_form_submit($form, &$form_state) {
  // Save the setting
  variable_set('commerce_multicurrency_tmt_feed', $form_state['values']['feed']);
  variable_set('commerce_multicurrency_tmt_markup_amount', $form_state['values']['markup_amount']);
  
  // Clear the cached exchange rates
  cache_clear_all('commerce_multicurrency_tmt_exchange_rate_sync_provider_tmt', 'cache');
  
  // Sync new exchange rates
  commerce_multicurrency_sync_exchange_rates_now();
}