<?php 
/**
 * @file the renren helper
 */

function renren_user_profile($renren_uid = null) {
  global $RR_client, $RR_config;
  if (isset($_SESSION[$RR_config->APIKey.'_renren_user'])) {
    return $_SESSION[$RR_config->APIKey.'_renren_user'];
  }
  if (!isset($renren_uid)) {
    $renren_uid = $_COOKIE[$RR_config->APIKey.'_user'];
  }
  $response = $RR_client->POST('users.getInfo', array($renren_uid, 'uid,name,sex,star,zidou,vip,birthday,email_hash,tinyurl,headurl,mainurl,hometown_location,work_info,university_info'));
  $_SESSION[$RR_config->APIKey.'_renren_user'] = $response[0];
  return $response[0];
}




/**
 * auto create account for renren
 */
function renren_account_auto_create($renren_user = null) {
  global $user;
  if (!isset($renren_user)) {
    $renren_user = renren_user_profile();
    if (!isset($renren_user)) {
      return false;
    }
  }
  // renren account is already exists
  if ($hishanghai_uid = hishanghai_account_uid_with_renren($renren_user->uid)) {
     $account = user_load($hishanghai_uid);
     if ($account) {
       $user = $account;
       drupal_goto('');
     }
     else {
       return;
     }
  }
  else {
     // renren account is not already exists
     // we create for user
    $user_edit = array(
      'name' => check_plain($renren_user->name),
      'mail' => 'renren@renren.com',
      'pass' => user_password(),
      'status' => 1,
      'init' => check_plain($renren_user->name),
      'access' => time(),
    );
    // check the user name 
    if ($account = user_load(array('name' => $user_edit['name']))) {
      drupal_set_message(t('Sorry, We already have %name in our %site', array('%name' => $account->name, '%site' => variable_get('site_name', ''))));
      return;
    }
    else {
      $account = user_save(NULL, $user_edit);
      if ($account->uid) {
         if (!renren_account_insert($account->uid, $renren_user->uid)) {
           drupal_set_message(t('Sorry, there have errors accours.Please contact site adminstrator for support'));
           return;
         }
         else {
           $user = $account;
           drupal_set_message(t('Please complete the follow register form to create account on %site', array('%site' => variable_get('site_name', ''))));
           drupal_goto("user/$account->uid/settings");
         }
      }
      else {
        drupal_set_message(t('Sorry, there have errors accors.Please contact site adminstrator for support'));
        return;
      }
    }
  }
}

/**
 * Login renren 
 */
function renren_login_form() {
  return _renren_login_form();
}

/**
 * Display user profile from renren
 * @param $user 
 */
function renren_user_login_receiver($user = null) {
  return _renren_user_login_receiver();
}

/**
 * this is the RENREN CODE that display profil
 */
function _renren_user_login_receiver() {
  return '<script src="http://static.connect.renren.com/js/v1.0/XdCommReceiver.jsp" type="text/javascript"></script>';
}

/**
 * the form for login renren
 */
function _renren_login_form() {
  global $RR_config, $user;
  //$backurl = url('user', array('absolute' => true));
  $xd_receiver = url("user/renren_receiver", array('absolute' => true));
  return
  '<xn:login-button autologoutlink="true"></xn:login-button>
    <script type="text/javascript" src="http://static.connect.renren.com/js/v1.0/FeatureLoader.jsp"></script>
    <script type="text/javascript">
      XN_RequireFeatures(["EXNML"], function()
      {
        XN.Main.init("'.$RR_config->APIKey.'", "'.$xd_receiver.'");
      });
    </script>';
}

/**
 * Check user login status
 */
function renren_user_logined() {
  global $RR_config;
  return isset($_COOKIE[$RR_config->APIKey.'_session_key']) || !empty($_COOKIE[$RR_config->APIKey.'_session_key']);
}