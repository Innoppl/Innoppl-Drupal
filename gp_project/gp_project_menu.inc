<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function gp_project_mc_project_page() {

    $sections = array();
    $sections['filter_block'] = _get_project_filters();
    $sections['content_block'] = _get_latest_projects();

    return theme('projects_landing_page',$sections);
}

function gp_project_mc_donorlist($gnid) {
    $gnode = node_load($gnid);
    $users = gp_project_get_donors($gnid);
    return theme('projectuserlist',$users, 'donor', $gnode->title);
}

function gp_project_mc_volunteerlist($gnid) {
    $gnode = node_load($gnid);
    $users = gp_project_get_volunteers($gnid);
    return theme('projectuserlist',$users, 'volunteer', $gnode->title);
}

/**
 * Callback of Share this project page.
 * @global <type> $user
 * @global <type> $base_url
 * @param <type> $nid
 * @return <type> 
 */
function gp_project_mc_share_project($nid = null) {
    global $user;
    global $base_url; 

    $node_info = node_load($nid);
    $proj_path = $base_url.'/'. drupal_get_path_alias('node/'.$nid);
    
    $facebook_link = "http://www.facebook.com/sharer.php?u=" . $proj_path."&t=".$node_info->title;
    $twitter_link = "http://twitter.com/home?status=Currently reading ". $proj_path." title=".$node_info->title;
   // $email_link = 'mailto:?body=' . $node_info->title . ' ' . $proj_path . '?utm_campaign=UserReferral&utm_medium=email&utm_source=givingpoint&subject=project from givingpoint: ' .$node_info->title;

    $email_link = drupal_get_form('og_invite_form',$nid);
    $output = theme('share_project',$node_info->title, $proj_path, $facebook_link, $email_link, $twitter_link);
    return $output;

}
