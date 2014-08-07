<?php

/**
 * implementation of the hosting_post_install hook
 */
function hosting_varnish_post_hosting_install_task($task, $data) {
  drush_log('HVPIT');

  if ($task->ref->type == 'site') {
    $hosting_name = $task->ref->hosting_name;

    drush_log('Enabling cache settings for ' . $hosting_name);

    $result = provision_backend_invoke ($hosting_name, "variable-set --exact cache 1");
    drush_log($result);
    $result = provision_backend_invoke ($hosting_name, "variable-set --exact cache_lifetime \"60\"");
    drush_log($result);
    $result = provision_backend_invoke ($hosting_name, "variable-set --exact page_cache_maximum_age \"60\"");
    drush_log($result);
  }
}
