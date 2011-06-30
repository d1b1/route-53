<?php
// $Id$
/**
 * @file
 * Administrator Form and settings.
 *
 * Functons that provides the settings for the module.
 */

/**
 * This is the settings for the contribution module.
 */
function amazon_r53_settings() {

  $form = array();

  $form['aws_r53'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#description'  => t('Provides AWS Route 53 Settings. Note: For dev work, the passwords and keys are not secure.'),
    '#collapsible'  => TRUE,
    '#title'        => t('AWS Route 53')
  );

  $form['aws_r53']['aws_r53_user'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter your AWS User Name.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_user', ''),
    '#title'         => t('AWS User Name')
  );

  $form['aws_r53']['aws_r53_key_id'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter the Amazon Access Key ID.'),
    '#collapsible'   => TRUE,
    '#required'      => TRUE,
    '#default_value' => variable_get('aws_r53_key_id', ''),
    '#title'         => t('AWS Access Key ID')
  );

  $form['aws_r53']['aws_r53_secret_key'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Please enter the Amazon Secret Key.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('aws_r53_secret_key', ''),
    '#title'         => t('AWS Access Key ID')
  );

  $form['aws_r53']['aws_r53_hosted_zone'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Please enter the default hosted zone. Optional for now.'),
    '#default_value' => variable_get('aws_r53_hosted_zone', ''),
    '#title'         => t('Hosted Zone')
  );

  $form['aws_r53']['aws_hide_record_types'] = array(
    '#type'          => 'textfield',
    '#description'   => t('If you want to hide by default certain types of records, then 
      the values with commas. Example MX etc. By keeping these out of the list, we lower
      the chance of an accidental error.'),
    '#default_value' => variable_get('aws_hide_record_types', 'MX'),
    '#title'         => t('Hide Record Types')
  );
  
  // Defaults Fieldset.
  $form['aws_r53_defaults'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#description'  => t('These are the default values that will be used on the Add Zone entry form.'),
    '#title'        => t('Zone Record Defaults')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_value'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Please enter the default value for new zone file records.'),
    '#default_value' => variable_get('aws_r53_default_record_value', ''),
    '#title'         => t('Default Record Value')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_ttl'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Please enter the default time to live for new zone file records.'),
    '#default_value' => variable_get('aws_r53_default_record_ttl', 100),
    '#title'         => t('Default Record TTL')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_type'] = array(
    '#type'          => 'select',
    '#description'   => t('Please enter the default time to live for new zone file records.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('aws_r53_default_record_type', 'CNAME'),
    '#title'         => t('Default Record Type. Recommended to set to the type defined by the default value.'),
    '#options'       => _amazon_r53_get_record_types(),
  );

  $form['aws_r53_defaults']['aws_helper_entry_values'] = array(
    '#type'          => 'textarea',
    '#description'   => t('Please enter the default time to live for new zone file records.'),
    '#default_value' => variable_get('aws_helper_entry_values', ''),
    '#title'         => t('Default Record TTL')
  );

  return system_settings_form($form);
}