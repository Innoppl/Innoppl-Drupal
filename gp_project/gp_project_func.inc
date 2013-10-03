<?php

/**
 *
 * @return <type>
 */
function gp_project_get_latest_projects() {

    $query = "select distinct(node.nid) from node node
                left join content_type_project project on project.nid = node.nid
                left join workflow_node wn on wn.nid = node.nid
                left join workflow_states ws on wn.sid = ws.sid
                left join workflows w on ws.wid = w.wid
                where node.type = 'project' and w.name = 'Promote Projects' and (ws.state = 'Active' or ws.state = 'Done' or ws.state = 'Partially Funded')
                and UNIX_TIMESTAMP(project.field_end_date_value) > UNIX_TIMESTAMP()
                order by (ws.state = 'Partially Funded') desc, abs(project.field_donation_value) desc, project.field_end_date_value asc";

    $count_query = "select count(distinct(node.nid)) from node node
                left join content_type_project project on project.nid = node.nid
                left join workflow_node wn on wn.nid = node.nid
                left join workflow_states ws on wn.sid = ws.sid
                left join workflows w on ws.wid = w.wid
                where node.type = 'project' and w.name = 'Promote Projects' and (ws.state = 'Active' or ws.state = 'Done')
                and UNIX_TIMESTAMP(project.field_end_date_value) > UNIX_TIMESTAMP()
                order by abs(project.field_donation_value) desc, project.field_end_date_value asc";

    $result_set = pager_query($query, 4, 0, $count_query);
    while($row = db_fetch_object($result_set)){

        $node = node_load($row->nid);
        $user = user_load($node->uid);
        profile_load_profile($user);

        $node_info['title'] = $node->title;
        $node_info['desc'] = $node->og_description;
       	$node_info['city'] = $node->field_proj_city[0]['value'];
        $node_info['end_date'] = strtotime($node->field_end_date[0]['value']);
        
        if(!empty($node->field_start_date[0]['value'])) {
            $node_info['event_date'] = format_date(strtotime($node->field_start_date[0]['value']), 'gp1');
            if(!empty($node->field_end_date[0]['value']) && $node->field_end_date[0]['value']!=$node->field_start_date[0]['value'] ) {
                   $node_info['event_date'].=" - ".format_date(strtotime($node->field_end_date[0]['value']), 'gp1');
            }
        }
        else {
            $node_info['event_date'] = '';
        }
        if(in_array('Giving Pointer',$user->roles)) {
            $node_info['author'] = l(gpapi_displayname($user), 'profile/'.$user->uid);
            $node_info['npo'] = l(gpapi_displaynponame($node), 'profile/'.$node->field_organization[0][value]);
            $node_info['author_uid'] = $node->field_organization[0][value];
        }
        if(in_array('Non-Profit Organization',$user->roles)) {
            $node_info['author'] = l($user->profile_organization_name, 'profile/'.$user->uid);
            $node_info['author_uid'] = $user->uid;
        }
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = $node->path;
        if(!empty($node->field_volunteers[0]['value'])) {
            $node_info['volunteers'] = $node->field_volunteers[0]['value'];
        }
        else {
            $node_info['volunteers'] = 0;
        }
        if(!empty($node->field_current_volunteers[0]['value'])) {
            $node_info['current_volunteers'] = $node->field_current_volunteers[0]['value'];
        }
        else {
            $node_info['current_volunteers'] = 0;
        }
        $node_info['donation_needed'] = get_project_donation_amount_needed($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid);
        $node_info['donation'] = get_total_approved_donations($node->nid);

        foreach($node->taxonomy as $key => $val) {
            if($val->vid == '4') {
                $node_info['loc'] = $val->name;
            }
        }
        $project_arr[] = $node_info;
    }
    $output['page'] = $project_arr;
    $output['pager'] = theme('pager');
    return $output;
}

/**
 *
 * @return <type> 
 */
function gp_project_get_project_filters () {

    $block_det['proj_filters'] = module_invoke('gpapi','block_embed','gp_blocks','filter_projects');
    $block_det['search_text'] = module_invoke('gpapi','block_embed','gp_blocks','filter_search_text');
    return theme('project_filter_box',$block_det);
}

/**
 * Function to get all donors for a project
 *
 * @param <type> $gnid
 */
function gp_project_get_donors($gnid) {
    $sql = "select distinct node.uid as uid, node_data_field_donation_amount.field_donation_amount_value As donation
            from node
            left join og_ancestry on og_ancestry.nid = node.nid
            left join content_type_project_donation on node.nid = content_type_project_donation.nid
            left join workflow_node wn on wn.nid = node.nid
	    left join content_field_is_anonymous on node.nid = content_field_is_anonymous.nid
	    LEFT JOIN content_field_donation_amount node_data_field_donation_amount ON node.vid = node_data_field_donation_amount.vid
            left join workflow_states ws on wn.sid = ws.sid
            left join workflows w on ws.wid = w.wid
            where node.type='project_donation' and w.name = 'Promote Contributions'
            and og_ancestry.group_nid = {$gnid} and (ws.state = 'Approved')
            and (content_field_is_anonymous.field_is_anonymous_value is null or
            content_field_is_anonymous.field_is_anonymous_value = 0)";

    $result = db_query($sql);
    $rows = array();
    while ($row = db_fetch_object($result) ) {
        $uid = user_load($row->uid);
        array_push($rows, $uid);
		$uid->donation = $row->donation;
    }
//echo "<pre>"; 
//print_r($rows);
    return $rows;
}

/**
 * Function to get all donors for a project
 *
 * @param <type> $gnid
 */
function gp_project_get_pledge($gnid) {
    $sql = "select node.uid as uid, content_type_make_a_pledge.field_pledge_amount_value AS donation from node
    left join content_type_make_a_pledge on node.nid = content_type_make_a_pledge.nid
    where content_type_make_a_pledge.field_pledge_project_value = {$gnid}";

    $result = db_query($sql);
    $rows = array();
    while ($row = db_fetch_object($result) ) {
        $uid = user_load($row->uid);
        array_push($rows, $uid);
		$uid->donation = $row->donation;
    }
//echo "<pre>"; 
//print_r($rows);
    return $rows;
}

/**
 * Function to get all anonymous donors  for a project
 *
 * @param <type> $gnid
 */
function gp_project_get_anony_donors($gnid) {
    $sql = "select distinct node.uid as uid, node_data_field_donation_amount.field_donation_amount_value As donation
            from node
            left join og_ancestry on og_ancestry.nid = node.nid
            left join content_type_project_donation on node.nid = content_type_project_donation.nid
            left join workflow_node wn on wn.nid = node.nid
	    left join content_field_is_anonymous on node.nid = content_field_is_anonymous.nid
	     LEFT JOIN content_field_donation_amount node_data_field_donation_amount ON node.vid = node_data_field_donation_amount.vid
            left join workflow_states ws on wn.sid = ws.sid
            left join workflows w on ws.wid = w.wid
            where node.type='project_donation' and w.name = 'Promote Contributions'
            and og_ancestry.group_nid = {$gnid} and (ws.state = 'Approved')
            and (content_field_is_anonymous.field_is_anonymous_value = 1)";

    $result = db_query($sql);
    $rows = array();
    while ($row = db_fetch_object($result) ) {
       $uid = user_load($row->uid);
      $uid->uid = "anonymous";
      
        array_push($rows, $uid);
	$uid->donation = $row->donation;
    }
   
    return $rows;
}

/**
 * Function to get all grants for a project
 *
 * @param <type> $gni
 */
function gp_project_get_grants($gnid) {

	$sql = "SELECT node.nid AS nid,
   users.uid AS users_uid,
   node_data_field_affiliated_npo_uid.field_affiliated_npo_uid_value AS node_data_field_affiliated_npo_uid_field_affiliated_npo_uid_value,
   node.type AS node_type,
   node.vid AS node_vid,
   node_data_field_affiliated_npo_uid.field_grant_amount_value AS grant_amount,
   node_data_field_affiliated_npo_uid.field_grant_other_npo_value AS node_data_field_affiliated_npo_uid_field_grant_other_npo_value,
   node.created AS node_created
 FROM node node 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid
 INNER JOIN users users ON node.uid = users.uid
 LEFT JOIN content_type_grant_request node_data_field_affiliated_npo_uid ON node.vid = node_data_field_affiliated_npo_uid.vid
 WHERE (node.type in ('grant_request')) 
 AND (node_data_field_affiliated_npo_uid.field_grant_project_gp_nid_value = ".$gnid." 
 OR node_data_field_affiliated_npo_uid.field_assoc_project_nid_value = ".$gnid."
 OR node_data_field_affiliated_npo_uid.field_grant_project_at_nid_value = ".$gnid."
 OR node_data_field_affiliated_npo_uid.field_grant_project_npo_nid_value = ".$gnid.")
 AND (node_data_field_affiliated_npo_uid.field_grant_amount_value !='') AND (workflow_node.sid = 18)
   ORDER BY node_created DESC";
      
   $result = db_query($sql);

    $rows = array();
	
    while ($row = db_fetch_object($result) ) {
			$user_uid = user_load($row->users_uid);
			//$user_uid['re'] = "TESTTING":
			$user_uid->donation = $row->grant_amount;
        array_push($rows, $user_uid);
//	echo	$suid['donation'] = $row->grant_amount;
//	array_push($rows, $suid['donation']);
	 }
//echo "<pre>"; 
//print_r($rows);
//exit;
    return $rows;
}



/** Function to get all volunteers for a project
 *
 * @param <type> $gnid
 */
function gp_project_get_volunteers($gnid) {
    $sql = "select * from og_uid ogu
            left join users_roles ur on ogu.uid = ur.uid
            left  join role r on ur.rid = r.rid
            where ogu.nid = $gnid
            and ogu.is_admin = 0
            and r.name = 'Giving Pointer'
            order by ogu.created";
    $result = db_query($sql, $gnid);
    $rows = array();
    while ($row = db_fetch_object($result) ) {
        array_push($rows, user_load($row->uid));
    }
    return $rows;
}

/**
 * Function to fetch photos of a project gallery
 * @global <type> $user
 * @param <type> $userid
 * @return <type> 
 */
function _get_project_photos($proj_albumid = null) {
    global $user;
   $proj_album_node = node_load($proj_albumid);
   $album_config = node_gallery_get_config($proj_album_node->type);
   //$vars['proj_album'] = theme('gallery_images_list', $proj_album_node, $album_config);

    $project_photos = node_gallery_get_gallery_images($proj_album_node);
    $config = node_gallery_get_config($proj_album_node->type);
    $sections = array('config'=>$config, 'images'=>$project_photos);
   
    return theme('project_photos_grid',$sections,"<h3>Project Gallery</h3>", "");
}