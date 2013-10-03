<?php
/**
 * This file will be used defining callbacks for the menu items of gp_homepage module.
 * The naming convention of the callback functions is gp_homepage_mc_MENUNAME
 *
 * @package givingpoint_package
 * @subpackage gp_homepage
 *
 * @category module
 */



/**
 * This function is the callback of the menu item /gpindex .It generates the homepage.
 * @return string : Output is the themed main content for the homepage of givingpoint.org
 *
 * 
 */

function gp_homepage_mc_gpindex(){
    drupal_add_css(drupal_get_path('module', 'gp_homepage') .'/theme/gp_homepage.css');
    $sections['slideshow']  = gp_homepage_slideshow();
    $sections['desc_right'] = gp_homepage_desc_right();
    $sections['newsblock']  = gp_homepage_newsblock();
    $sections['desc_middle']  = gp_homepage_desc_middle();
    $sections['newsletter'] = gp_homepage_newsletter();
    return theme('gpindex_page', $sections);
}

/**
 *
 * @return <type>
 *
 */
function gp_homepage_mc_about() {

    drupal_add_css(drupal_get_path('module', 'gp_homepage') .'/theme/gp_homepage.css');
    $assoc_page_id = variable_get('about','');
    if(!empty($assoc_page_id)) {
        $node = node_load($assoc_page_id);
        drupal_set_title(check_plain($node->title));
        return node_show($node, 0);
    }
}

/**
 *
 * @return <type>
 *
 */
function gp_homepage_mc_for_parents() {

    drupal_add_css(drupal_get_path('module', 'gp_homepage') .'/theme/gp_homepage.css');
    $assoc_page_id = variable_get('for_parents','');
    if(!empty($assoc_page_id)) {
        $node = node_load($assoc_page_id);
        drupal_set_title(check_plain($node->title));
        return node_show($node, 0);
    }
}

/**
 *
 * @return <type>
 *
 */
function gp_homepage_mc_for_teachers() {

    drupal_add_css(drupal_get_path('module', 'gp_homepage') .'/theme/gp_homepage.css');
    $assoc_page_id = variable_get('for_teachers','');
    if(!empty($assoc_page_id)) {
        $node = node_load($assoc_page_id);
        drupal_set_title(check_plain($node->title));
        return node_show($node, 0);
    }
}

/**
 *
 * @return <type>
 *
 */
function gp_homepage_mc_for_non_profits() {

    drupal_add_css(drupal_get_path('module', 'gp_homepage') .'/theme/gp_homepage.css');
    $assoc_page_id = variable_get('for_non_profits','');
    if(!empty($assoc_page_id)) {
        $node = node_load($assoc_page_id);
        drupal_set_title(check_plain($node->title));
        return node_show($node, 0);
    }
}


