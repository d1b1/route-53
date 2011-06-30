<?php

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