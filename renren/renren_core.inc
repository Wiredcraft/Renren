<?php
// ;$id
/**
 * @file renren core file
 */

/**
 * 
 * return renren config object
 */
function renren_config() {
  $config       = new stdClass;

  $config->APIURL   = 'http://api.xiaonei.com/restserver.do'; //RenRen网的API调用地址，不需要修改
  $config->APIKey   = ''; //你的API Key，请自行申请
  $config->SecretKey  = ''; //你的API 密钥
  $config->APIVersion = '1.0';  //当前API的版本号，不需要修改
  $config->decodeFormat = 'json'; //默认的返回格式，根据实际情况修改，支持：json,xml
  
  /**把所有的参数，包括required及optional，除了api_key,method,v,format不需要填写之外，
     其它的都可以根据你的实现情况来处理，以英文半角状态下的逗号来分割各个参数。 **/
  $config->APIMapping   = array( 
    'admin.getAllocation' => '',
    'connect.getUnconnectedFriendsCount' => '',
    'friends.areFriends' => 'uids1,uids2',
    'friends.get' => 'page,count',
    'friends.getFriends' => 'page,count',
    'notifications.send' => 'to_ids,notification',
    'users.getInfo' => 'uids,fields',
    'feed.publishTemplatizedAction' => 'template_id,title_data,body_data',
  );
  return $config;
}


/**
 * Logout renren user account
 */
function renren_user_destory() {
  unset($_SESSION[$RR_config->APIKey.'_renren_user']);
}

/**
 * Post comment to renren 
 */
function renren_comment(&$comment, $op) {
  global $user;
  if (!is_array($comment)) {
    $comment = (array)$comment;
  }
  $comment_body = renren_clean_data($comment['comment']);
  $comment_subject = renren_clean_data($comment['subject']);
  
  if ($body_pattern = variable_get('renren_template_comment_body', '')) {
     $body = '{"'.$body_pattern.'" : "'.$comment_body.'"}';
  }

  if ($title_pattern = variable_get('renren_template_comment_title', '')) {
    $title = '{"'.$title_pattern.'" : "'.$comment_subject.'"}';
  }
  //$body = '{"content":"'.$comment_body.'"}';
  //$title = '{"feedtype":"'.$comment_subject.'"}';
  
  // if renren user logined
  $comment_author = renren_user_profile();
  if (isset($comment_author)) {
    global $RR_client;
    $result = $RR_client->POST('feed.publishTemplatizedAction', array(1, $title, $body));
    if ($result->result) {
      drupal_set_message('Successful share this comment on renren');
    }
    else {
      drupal_set_message('Renren Error: '.$result->error_msg);
      drupal_set_message('Renren Error Code: '.$result->error_code);
    }
  }
}