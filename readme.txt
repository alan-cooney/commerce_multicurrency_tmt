Trust My Travel Currency API

DEPENDENCIES
Commerce Multicurrency Trust My Travel depends on the Commerce multicurrency 
module.

INSTALLATION
Install the module as usual.
Setup cron to run at 12.30.05, 12.31 and 12.35 GMT to make sure that all currency updates are run.

CONFIGURATION
1. Choose to use the staging or live (default) feed at /admin/commerce/config/currency/tmt
2. Sync rates at /admin/commerce/config/currency/conversion

IMPORTANT NOTES
This module currently has no way of dealing with currencies which are not provided by TMT.