<?php
// $Id$

/**
 * @file
 * Helper functions for the r53 class.
 *
 * All helper functions that provides the interface with the r53 API
 * and the r53 class.
 */

/**
 * This is a helper function that will filter the site aliases
 * to only show options that fall in the zonefile base domain.
 * 
 * Example: "/mydomain\.com$/
 */
function _amazon_r53_filter( $aliases, $hosted_zone_info ) {

  $pattern = '/' . preg_quote(preg_replace('#\.$#', '', $hosted_zone_info['Name']), '#') . '$/';

  $return = array();
  foreach ($aliases as $idx => $alias) {
    if (preg_match($pattern, $alias)) {
      $return[$alias . '.'] = $alias . '.';
    }

  }

  return $return;
}
 
 
/**
 * Helper function that returns a clean value 
 * from the raw name. This makes the @, * readable.
 * Also removes the trailing . Its required, but does
 * not help readility.
 */
function _amazon_r53_clean( $value ) {
  
  // Define the patterns regexs
  $patterns = array();
  $patterns[0] = '#\\\052#';
  $patterns[1] = '#\\\100#';
  $patterns[2] = '#\.$#';
  
  // Define the replacements values.
  $replacements = array();
  $replacements[2] = '*';
  $replacements[1] = '@';
  $replacements[0] = '';

  return preg_replace($patterns, $replacements, $value);
}

/**
 * This is a helper function that will return the apex 
 * record for the zone file. This is called by the _info
 * to determine the failback when there is no specific 
 * CName for a site alias.
 */
function _amazon_r53_get_wildcard() {
  
  // Call the lookup function for the wildcard
  $result = _amazon_r53_lookup_site('wildcard');
  
  if ($result['status']) {
    return $result['entry']['0'];
  }
  
  return FALSE;
}

/**
 * This is a helper function that will return the apex 
 * record for the zone file. This is called by the _info
 * to determine the failback when there is no specific 
 * CName for a site alias.
 */
function _amazon_r53_get_root() {
  
  // Call the lookup function for the wildcard
  $result = _amazon_r53_lookup_site('root');
  
  if ($result['status']) {
    return $result['entry']['0'];
  }
  
  return FALSE;
}

/**
 * This function gets all the types.
 */
function _amazon_get_type_name( $type ) {

  $types = _amazon_r53_get_record_types();

  return $types[$type];
}

/**
 * For now this will provide the user with a help message
 * and give them the option to get to the settings page.
 */
function _amazon_r53_message( $op ) {
  
  switch ($op) {
    case 'missing settings':
      drupal_set_message(t('AWS Settings missing. Before you can use this feature, you need to provide some basics.') . l('AWS Routhe 53 Settings', 'admin/settings/r53/settings'));
      break;
      
    default:
      drupal_set_message(t('What up, no op :)'));
      break;
  }

}

/**
 * This function will get the recordset information for a specific record
 * in the Hosted Zone
 */
function _amazon_r53_get_record( $record ) {

  $r53 = _amazon_r53_get_service();

  $result = $r53->listResourceRecordSets(_amazon_get_current_hosted_zone());
  $esource_record_sets = $result['ResourceRecordSets'];

  $return = NULL;

  // This need to be refactored. The . is a hack for now.
  foreach ($esource_record_sets as $index => $entry) {
    if ($entry['Name'] == $record . '.') {
      $return = $entry;
    }
  }

  return $return;
}

/*
 * This function will attempt to match the site name to the
 * and the aliases to any existing DNS records.
 *
 */
function _amazon_r53_lookup_site($op = 'byname', $aliases = array() ) {

  // Get the Service.
  $r53 = _amazon_r53_get_service();

  // Call the Service
  $result = $r53->listResourceRecordSets(_amazon_get_current_hosted_zone());
  $resource_record_sets = $result['ResourceRecordSets'];

  // Init the return values.
  $return    = array();
  $found     = array();
  $notmatched = array();

  switch (strtolower($op)) {
    case 'wildcard':
      // Look for values starting with * or \052.
      foreach ($resource_record_sets as $index => $entry) {
        if (preg_match('#^\\\052#', $entry['Name'])) {
          $return[] = $entry;
          $found[] = _amazon_r53_clean($entry['Name']);
        }
      }
      break;

    case 'root':
      // Look for values starting with @ or \100.
      foreach ($resource_record_sets as $index => $entry) {
        if (preg_match('#^\\\100#', $entry['Name'])) {
          $return[] = $entry;
          $found[] = _amazon_r53_clean($entry['Name']);
        }
      }
      break;

    case 'bytype':
      // Find by Entry Type: MX, CNAME, A
      if (!is_array($aliases)) {
        $aliases = array($aliases); 
      }

      foreach ($resource_record_sets as $index => $entry) {
        if (in_array(_amazon_r53_clean($entry['Type']), $aliases)) {
          $return[] = $entry;
          $found[] = _amazon_r53_clean($entry['Name']);
        }
      }
      $notmatched = array_diff($aliases, $found);
      break;

    case 'byname':
      // Find by name.
      foreach ($resource_record_sets as $index => $entry) {
        if (in_array(_amazon_r53_clean($entry['Name']), $aliases)) {
          $return[] = $entry;
          $found[] = _amazon_r53_clean($entry['Name']);
        }
      }

      $notmatched = array_diff($aliases, $found);
      break;
      
    case 'byvalue':
      // Find by value. This will loop the records and match
      // any entries by specific value.
     if (!is_array($aliases)) {
        $aliases = array($aliases); 
      }

      foreach ($resource_record_sets as $index => $entry) {
        if (in_array(_amazon_r53_clean($entry['ResourceRecords'][0]), $aliases)) {
          $return[] = $entry;
          $found[] = $entry['ResourceRecords'][0];
        }
      }

      $notmatched = array_diff($aliases, $found);
      break;
  }

  $notmatched = array_diff($aliases, $found);
  return array( 'status' => !empty($return), 'entry' => $return, 'unmatched' => $notmatched);
}

/**
 * Helper function that gets the service and returns the available zones.
 * 
 * TODO: If we had user security from AWS IAM, figure out how to
 * limit access to specific zone files.
 */
function _amazon_get_hosted_zones() {

  $r53 = _amazon_r53_get_service();
  if (empty($r53)) {
    _amazon_r53_message('missing settings');
    return '';
  }

  $result = $r53->listHostedZones();

}

/**
 * This function checks that the Zone is defined. If not defined
 * this will push the user to the list of zone files. Coming soon...
 */
function _amazon_get_current_hosted_zone() {

  $hosted_zone = $_SESSION['hosted_zone_id']; //variable_get('aws_r53_hosted_zone','');

  if (!$hosted_zone) {
    // TODO: Rethink this. forcing the user to the settings is not good for 
    // drush.
    drupal_goto('admin/settings/r53/zones');
  }

  return $hosted_zone;
}

/**
 * This function returns a valid list of Record Types.
 * 
 * TODO remember to use to validation the types in the lookup and the form submit.
 */
function _amazon_r53_get_record_types() {

  return array(
    'A'     => 'A Record',
    'AAAA'  => 'AAAA Record',
    'CNAME' => 'CName',
    'MX'    => 'MX',
    'NS'    => 'NS',
    'PTR'   => 'PTR',
    'SOA'   => 'SOA',
    'SPF'   => 'SPF',
    'SRV'   => 'SRV',
    'TXT'   => 'TXT',
  );

}

/**
 * This function returns the value for change request.
 */
function _amazon_r53_change_resource_record( $requested_changes ) {

  $r53 = _amazon_r53_get_service();
  $hosted_zone = _amazon_get_current_hosted_zone();

  $changes = array();
  foreach ($requested_changes as $key => $value) {
    $changes[] = $r53->prepareChange($value['mode'], $value['name'], $value['type'], $value['ttl'], $value['value']);
  }

  $result = $r53->changeResourceRecordSets($hosted_zone, $changes);

  if (!empty($result)) {
    if ($result['Status']=='PENDING') {
      $status = TRUE;
      $message = 'Change is processing.';
      $id = $result['Id'];
    }
    else {
      $status = FALSE;
      $message = 'Unknown';
      $id = 'NA';
    }
  }
  else {
    $status = FALSE;
    $message = 'Failed';
    $id = 'NA';
  }

  return array('status' => $status, 'message' => $message, 'id' => $id);
}

/**
 * This function gets the base R53 Class. check to make 
 * sure the values are set. If not return a null. 
 * 
 * TODO: Refactor the code to catch errors on the class
 * TODO: Valdation the function.
 */
function _amazon_r53_get_service() {

  // Load the r53.php class into the module
  require_once('r53.php');
  
  $id  = variable_get('aws_r53_key_id', '');
  $key = variable_get('aws_r53_secret_key', '');
  
  if (empty($id) || empty($key)) {
    return NULL;
  }
  
  $r53 = new Route53($id, $key);
  
  return $r53;
}


/**
 * Helper function to manage the site tab for DNS entires
 */
function _amazon_r53_access( $node ) {

  if (empty($node)) {
    return FALSE;
  }

  return ($node->type=='site');
}

/**
 * this is a helper that gets the current zone record
 * for use in the add/edit form.
 */
function _amazon_r53_get_zone_info($find_zone) {

  $hosted_zone = _amazon_get_current_hosted_zone();
  $r53 = _amazon_r53_get_service();
  
  $r53 = _amazon_r53_get_service();
  if (empty($r53)) {
    return NULL;
  }

  $result = $r53->listHostedZones();

  foreach ($result['HostedZone'] as $zone) {
    if ($zone['Id']==$find_zone) {
      return $zone;
    }

  }
  
  return NULL;
}
