<?php

/**
 * implementation of the hosting_post_install hook
 */
function hosting_varnish_post_hosting_install_task($task, $data) {
  if ($task->ref->type == 'site') {
    $hosting_name = $task->ref->hosting_name;

    drush_log('Enabling cache settings for ' . $hosting_name);
    
    $cache_vars = array(
      'cache',
      'block_cache',
      'cache_lifetime',
      'page_cache_maximum_age',
      'page_compression',
      'preprocess_css',
      'preprocess_js',
    );

    // Iterate over possible variables and set them in the site
    foreach ($cache_vars as $varname) {
      $hosting_varnish_varname = 'hosting_varnish_' . $varname . '_' . $task->ref->platform;
      $value = variable_get($hosting_varnish_varname, NULL);

      if ($value !== NULL) {
        drush_log('[hosting_varnish] ' . $hosting_varnish_varname . ': ' . $value);
        provision_backend_invoke($hosting_name, "variable-set --exact " . $varname . ' ' . $value);
      }
      else {
        drush_log('[hosting_varnish] NULL: ' . $hosting_varnish_varname);
      }
    }
  }
}
