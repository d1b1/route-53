<?php

/**
 * @file
 *   The hosting feature definition for the backup queue.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function amazon_r53_hosting_feature() {

  $features['hosting_aws_r53'] = array(
    'title'       => t('Amazon Route 53'),
    'description' => t('This feature provides access to the Amazon Route 53 DNS Service.'),
    'status'      => HOSTING_FEATURE_DISABLED,
    'module'      => 'amazon_r53',
    'weight'      => 100,
  );

  return $features;
}

