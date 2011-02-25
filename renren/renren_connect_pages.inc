<?php

/**
 * Setting page for renren connection variable: API and Secret
 * @param $form_state
 */
function renren_connection_settings(&$form_state) {
  $forms = array();
  $forms['renren_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Renren API settings'),
    '#description' => t('Please config API for renren if you want to connect renren server'),
  );
  
  // renren api settings
  $forms['renren_settings']['renren_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('renren_api_key', ''),
    '#description' => t('You should input the api key to access renren sever. You can get it from '.l('here', 'www.dev.renren.com', array('absolute' => true))),
  );
  $forms['renren_settings']['renren_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Secret'),
    '#default_value' => variable_get('renren_api_secret', ''),
    '#description' => t('Please input the API Secret. You should have it for accessing the user data'),
  );
  
  // comments
  $forms['renren_settings']['renren_comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose and select renren comment settings'),
  );
  $forms['renren_settings']['renren_comment']['renren_enable_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose and select renren comment settings'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('renren_enable_types', array()), 
  );
  $forms['renren_settings']['renren_comment']['renren_enable_comment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable share comment with Renren'),
    '#description' => t('If you checked, user can send her/his comment to Renren'),
    '#default_value' => variable_get('renren_enable_comment', 0),
  );
  
  //flag
  $forms['renren_settings']['renren_flag'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flag types settings. Choose which flag type will be catch to post renren'),
  );
  $forms['renren_settings']['renren_flag']['renren_flag_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose flag type'),
    '#options' => custom_renren_connect_get_flags_name(),
    '#default_value' => variable_get('renren_flag_types', array()),
  );
  $forms['renren_settings']['renren_enable_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable share like action with Renren'),
    '#default_value' => variable_get('renren_enable_flag', 0),
    '#description' => t('If you checked, user can send like action information to Renren'),
  );
  return system_settings_form($forms);
}

/**
 * Settings page for Renren action template
 * Enter description here ...
 */
function renren_action_template_settings() {
  $forms = array(
    'renren_template_comment_title' => array(
      '#title' => t('The comment title will be display at renren action'),
      '#type' => 'textfield', 
      '#default_value' => variable_get('renren_template_comment_title', ''),
      '#description' => t('If you want to the comment title be display at renren action,
                          you should set action template pattern for it. Example: title'),
    ),
    'renren_template_comment_body' => array(
      '#title' => t('The comment body will be displayed at renren action'),
      '#type' => 'textfield',
      '#default_value' => variable_get('renren_template_comment_body', ''),
      '#description' => t('If you want to the comment body be displayed at renren action,
                           you should set action template pattern for it. Example: body'),
    ),
  );
  return system_settings_form($forms);
}