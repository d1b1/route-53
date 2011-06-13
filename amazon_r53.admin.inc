<?php

/**
 * This is the settings for the contribution module.
 */
function amazon_r53_settings() {

  $form = array();

  $form['aws_r53'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#description' => t('Provides AWS Route 53 Settings.'),
    '#collapsible' => TRUE,
    '#title' => t('AWS Route 53')
  );

  $form['aws_r53']['aws_r53_user'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter your AWS User Name.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_user',''),
    '#title'         => t('AWS User Name')
  );

  $form['aws_r53']['aws_r53_key_id'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter the Amazon Access Key ID.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_key_id',''),
    '#title'         => t('AWS Access Key ID')
  );

  $form['aws_r53']['aws_r53_secret_key'] = array(
    '#type' => 'textfield',
    '#collapsed' => FALSE,
    '#description' => t('Please enter the Amazon Secret Key.'),
    '#collapsible' => TRUE,
    '#default_value' => variable_get('aws_r53_secret_key',''),
    '#title' => t('AWS Access Key ID')
  );

  $form['aws_r53']['aws_r53_hosted_zone'] = array(
    '#type' => 'textfield',
    '#collapsed' => FALSE,
    '#description' => t('Please enter the default hosted zone.'),
    '#collapsible' => TRUE,
    '#default_value' => variable_get('aws_r53_hosted_zone',''),
    '#title' => t('Hosted Zone')
  );

  // Defaults Fieldset.
  $form['aws_r53_defaults'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#description' => t('Zone Record Defaults'),
    '#collapsible' => TRUE,
    '#title' => t('Zone Record Defaults')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_value'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter the default value for new zone file records.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_default_record_value', ''),
    '#title'         => t('Default Record Value')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_ttl'] = array(
    '#type'          => 'textfield',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter the default time to live for new zone file records.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_default_record_ttl', 100),
    '#title'         => t('Default Record TTL')
  );

  $form['aws_r53_defaults']['aws_r53_default_record_type'] = array(
    '#type'          => 'select',
    '#collapsed'     => FALSE,
    '#description'   => t('Please enter the default time to live for new zone file records.'),
    '#collapsible'   => TRUE,
    '#default_value' => variable_get('aws_r53_default_record_type', 'CNAME'),
    '#title'         => t('Default Record Type. Recommended to set to the type defined by the default value.'),
    '#options'       => _amazon_r53_get_record_types(),
  );

  return system_settings_form($form);
}