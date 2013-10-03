<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @param <type> $user_id
 * @return <type>
 *
 */
function _get_filtered_projects($state = '', $city = '', $school = '', $author = '', $name = '', $from_date = '', $to_date = '', $organization = '') {
    
    $projects = array();  

    $join = " left join content_type_project project on project.nid = node.nid
            left join workflow_node wn on wn.nid = node.nid
            left join workflow_states ws on wn.sid = ws.sid
            left join workflows w on ws.wid = w.wid";
    $join .= " Left join profile_values profile_values on profile_values.uid = node.uid ";
    $join .= " Left join profile_fields profile on profile.fid = profile_values.fid ";
    $where = " WHERE (node.type in ('project')) and w.name = 'Promote Projects' and (ws.state = 'Active' or ws.state = 'Done' or  ws.state = 'Fully Funded' or  ws.state = 'Done with Partial Funding' or  ws.state = 'Partially Funded' or  ws.state = 'Done with No Funds') 
    		and UNIX_TIMESTAMP(project.field_end_date_value) > UNIX_TIMESTAMP() ";
    $order_by = " order by abs(project.field_current_donation_value) desc, project.field_end_date_value asc";
    
    if(!empty($state)) {
        $join .= " Left join term_node term_node on node.nid = term_node.nid";
        $join .= " Left join term_data term_data on term_node.tid = term_data.tid";
        $where .= "AND (term_data.tid = $state) AND (term_data.vid = '4') ";
    }
    if(!empty($city)) {
        
        $where .= " AND project.field_proj_city_value LIKE \"%$city%\" ";
    }
    if(!empty($school)) {
       
           $where .= " AND project.field_non_profit_organisation_value LIKE \"%$school%\" ";
    }
   
    if(!empty($author)) {
        $join .= " Left join users user on node.uid = user.uid";
        $where .= " AND (user.mail LIKE \"%$author%\") ";
    }
    if(!empty($name)) {
        $name = strtoupper($name);
        $where .= " AND (((profile_values.value like \"%$name%\") AND (profile.name = 'profile_first_name')) || ((profile_values.value like \"%$name%\") AND (profile.name = 'profile_contact_first_name'))) ";
    }
    //echo $from_date;
    if(!empty($from_date)) {        
        $from_date_array = explode('-', $from_date);
        //print_r($from_date_array);
        $from_date_timestamp = gmmktime(0,0,0,$from_date_array[1],$from_date_array[2],(int)$from_date_array[0]);
        
        if(empty($to_date)) {
            $to_date_timestamp = time();
        }
        else {
            $to_date_array = explode('-', $to_date);
            $to_date_timestamp = gmmktime(0,0,0,$to_date_array[1],$to_date_array[2],(int)$to_date_array[0]);
        }
            
        $where .= "AND node.created between '$from_date_timestamp' and '$to_date_timestamp' ";

    }

    if(!empty($organization)) {           
         $where .= "AND project.field_non_profit_organisation_value LIKE \"%$organization%\" ";      
         //$where .= "AND (profile_values.value like \"%$organization%\") AND (profile.name = 'profile_organization_name') ";

    }


    $query = "SELECT distinct(node.nid) FROM node node";
    $query .= $join;
    $query .= $where;
    $query .= $order_by;
    
    $count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
   
    while($row = db_fetch_object($result_set)) {
        
        $node = node_load($row->nid);
        $user = user_load($node->uid);
        profile_load_profile($user);
        
        $node_info['title'] = $node->title;
        $node_info['desc'] = $node->og_description;
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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid);
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_low_cost_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  
 LEFT JOIN term_node term_node ON node.vid = term_node.vid 
 LEFT JOIN content_type_project project on project.nid = node.nid
 LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
 INNER JOIN users users ON node.uid = users.uid 
 LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
 LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
 WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(7, 34) AND (project.field_donation_value <> 0)
 ORDER BY (project.field_donation_value)
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_most_donors_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  
 LEFT JOIN term_node term_node ON node.vid = term_node.vid 
 LEFT JOIN content_type_project project on project.nid = node.nid
 LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
 INNER JOIN users users ON node.uid = users.uid 
 LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
 LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
 WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(32,33,34) 
 ORDER BY (select count(n1.uid) as count_uid
            from node n1
            left join og_ancestry on og_ancestry.nid = n1.nid
            left join content_type_project_donation on n1.nid = content_type_project_donation.nid
            left join workflow_node wn on wn.nid = n1.nid
			left join content_field_is_anonymous on n1.nid = content_field_is_anonymous.nid
            left join workflow_states ws on wn.sid = ws.sid
            left join workflows w on ws.wid = w.wid
            where n1.type='project_donation' and w.name = 'Promote Contributions'
            and (og_ancestry.group_nid =node.nid) and (ws.state = 'Approved')
            and (content_field_is_anonymous.field_is_anonymous_value is null or
            content_field_is_anonymous.field_is_anonymous_value = 0)) DESC
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid);
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_few_days_project($search_text = ''){

	$query = " SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  
 LEFT JOIN term_node term_node ON node.vid = term_node.vid 
 LEFT JOIN content_type_project project on project.nid = node.nid
 LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
 INNER JOIN users users ON node.uid = users.uid 
 LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
 LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
 WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(7, 34) 
 ORDER BY (UNIX_TIMESTAMP(project.field_end_date_value) - UNIX_TIMESTAMP())
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_partial_fund_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  
 LEFT JOIN term_node term_node ON node.vid = term_node.vid 
 LEFT JOIN content_type_project project on project.nid = node.nid
 LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
 INNER JOIN users users ON node.uid = users.uid 
 LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
 LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
 WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(33, 34) ORDER BY field(workflow_node.sid,34,33)
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_full_fund_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  
 LEFT JOIN term_node term_node ON node.vid = term_node.vid 
 LEFT JOIN content_type_project project on project.nid = node.nid
 LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
 INNER JOIN users users ON node.uid = users.uid 
 LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
 LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
 WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(32) 
 ORDER BY (UNIX_TIMESTAMP(project.field_end_date_value) - UNIX_TIMESTAMP()) desc
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 *
 */
function _get_search_urgent_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, 
node.created AS node_created FROM node node  
LEFT JOIN content_type_project project on project.nid = node.nid
LEFT JOIN term_node term_node ON node.vid = term_node.vid 
LEFT JOIN term_data term_data ON term_node.tid = term_data.tid 
LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid 
INNER JOIN users users ON node.uid = users.uid 
LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' 
LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' 
LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid 
LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid 
WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND (((UNIX_TIMESTAMP(project.field_end_date_value) - UNIX_TIMESTAMP())/86400) <10) AND (((UNIX_TIMESTAMP(project.field_end_date_value) - UNIX_TIMESTAMP())/86400) >0) AND workflow_node.sid in(7, 34) ORDER BY node_created DESC
";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        

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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 * 
 */
function _get_search_funded_project($search_text = ''){

	$query = "SELECT distinct(node.nid), workflow_states.state AS workflow_states_state, node.created AS node_created FROM node node  LEFT JOIN term_node term_node ON node.vid = term_node.vid LEFT JOIN term_data term_data ON term_node.tid = term_data.tid LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid INNER JOIN users users ON node.uid = users.uid LEFT JOIN profile_values profile_values_profile_last_name ON users.uid = profile_values_profile_last_name.uid AND profile_values_profile_last_name.fid = '14' LEFT JOIN profile_values profile_values_profile_first_name ON users.uid = profile_values_profile_first_name.uid AND profile_values_profile_first_name.fid = '13' LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid WHERE (node.type in ('project')) AND (term_data.vid in ('3')) AND workflow_node.sid in(32, 33, 34) ORDER BY FIELD(workflow_states_state,'Partially Funded','Fully Funded','Done with Partial Funding'), node_created DESC";
	$count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
    
    while($row = db_fetch_object($result_set)) {
        
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
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

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

        if(!empty($node->field_donation[0]['value'])) {
            $node_info['donation_needed'] = $node->field_donation[0]['value'];
        }
        else {
            $node_info['donation_needed'] = 0;
        }

        $node_info['donation'] = get_total_approved_donations($node->nid);
        $node_info['grant_raised'] = get_total_approved_grants($node->nid); 
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
 * @param <type> $search_text
 * @return <type>
 * 
 */
function _get_search_filtered_project($search_text = '',$passion = '') {

    if(!empty($search_text) || !empty($passion)) {

        $search_key = $search_text. " type:project";
        $search_pass = $passion;
        $search_results = module_invoke('gpapi', 'search', 'search', $search_key,$search_pass, 4);
        
        if(!empty($search_results)) {
            foreach($search_results as $key => $result) {
                if(strtotime($result['node']->field_end_date[0]['value']) < time() OR strtotime($result['node']->field_end_date[0]['value']) > time()) {
                
                $user = user_load($result['node']->uid);
                profile_load_profile($user);
                
                $node_info['title'] = $result['node']->title;
                $node_info['desc'] = $result['node']->og_description;
                $node_info['end_date'] = strtotime($node->field_end_date[0]['value']);
                
                $node=$result["node"];
                if(!empty($node->field_start_date[0]['value'])) {
                   $node_info['event_date'] = format_date(strtotime($node->field_start_date[0]['value']), 'gp1');
//                   if(!empty($node->field_end_date[0]['value']) && $node->field_end_date[0]['value']!=$node->field_start_date[0]['value'] ) {
//                            $node_info['event_date'].=" - ".format_date(strtotime($node->field_end_date[0]['value']), 'gp1');
//                   }
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
            //    $node_info['author'] = $user->profile_public_name;
                $node_info['pri_image'] = theme('imagecache','113x113',$result['node']->field_primary_image[0]['filepath']);
                $node_info['node_path'] = drupal_get_path_alias('node/'.$result['node']->nid);

                if(!empty($result['node']->field_volunteers[0]['value'])) {
                    $node_info['volunteers'] = $result['node']->field_volunteers[0]['value'];
                }
                else {
                    $node_info['volunteers'] = 0;
                }

                if(!empty($result['node']->field_current_volunteers[0]['value'])) {
                    $node_info['current_volunteers'] = $result['node']->field_current_volunteers[0]['value'];
                }
                else {
                    $node_info['current_volunteers'] = 0;
                }

                if(!empty($result['node']->field_donation[0]['value'])) {
                    $node_info['donation_needed'] = $result['node']->field_donation[0]['value'];
                }
                else {
                    $node_info['donation_needed'] = 0;
                }

                $node_info['donation'] = get_total_approved_donations($result['node']->nid);
                $node_info['grant_raised'] = get_total_approved_grants($node->nid);
                foreach($result['node']->taxonomy as $key => $val) {
                    if($val->vid == '4') {
                        $node_info['loc'] = $val->name;
                    }
                }

                $project_arr[] = $node_info;
                }
            }
        }

        $output['page'] = $project_arr;
        $output['pager'] = theme('pager');
        return $output;


    }
}

/**
 *
 * @param <type> $project_id
 * @return <type> 
 */
function get_total_approved_donations($project_id)
{
    $amount = 0;

    $query = "select sum(content_field_donation_amount.field_donation_amount_value) as total
              from node
              left join content_type_project_donation on node.nid = content_type_project_donation.nid
              left join og_ancestry on og_ancestry.nid = node.nid 
              left join workflow_node wn on wn.nid = node.nid
              left join workflow_states ws on wn.sid = ws.sid
              left join workflows w on ws.wid = w.wid
	      left join content_field_donation_amount on node.nid = content_field_donation_amount.nid
              where node.type='project_donation' and w.name = 'Promote Contributions'
              and og_ancestry.group_nid = {$project_id} and (ws.state = 'Approved')";

    $result_set = db_fetch_object(db_query($query));
    if ($result_set)
    {
       if (!empty($result_set->total)){
           $amount = $result_set->total;
       }
    }

    return $amount;
}

/**
 * Total grant approved for a peoject
 * @param <type> $project_id
 * @return <type> 
 */
function get_total_approved_grants($project_id)
{
    $amount = 0;

    $query = "SELECT node_data_field_affiliated_npo_uid.field_grant_amount_value AS grant_amount
    FROM node node 
 INNER JOIN users users ON node.uid = users.uid
 LEFT JOIN content_type_grant_request node_data_field_affiliated_npo_uid ON node.vid = node_data_field_affiliated_npo_uid.vid
 LEFT JOIN workflow_node workflow_node ON node.nid = workflow_node.nid
 LEFT JOIN workflow_states workflow_states ON workflow_node.sid = workflow_states.sid
 LEFT JOIN node_revisions node_revisions ON node.vid = node_revisions.vid
 LEFT JOIN profile_values profile_values_profile_grants_left ON users.uid = profile_values_profile_grants_left.uid AND profile_values_profile_grants_left.fid = '7'
 WHERE (node.type in ('grant_request')) 
 AND (node_data_field_affiliated_npo_uid.field_assoc_project_nid_value = ". $project_id ."
		OR node_data_field_affiliated_npo_uid.field_grant_project_at_nid_value = ". $project_id ." 
		OR node_data_field_affiliated_npo_uid.field_grant_project_npo_nid_value = ". $project_id ." 
		OR node_data_field_affiliated_npo_uid.field_grant_project_gp_nid_value = ". $project_id .")
 AND (node_data_field_affiliated_npo_uid.field_grant_amount_value !='') AND (workflow_node.sid = 18)";
 
 $result_set = db_fetch_object(db_query($query));
    if ($result_set)
    {
       if (!empty($result_set->grant_amount)){
           $amount = $result_set->grant_amount;
       }
    }

    return $amount;
}



/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_project_donation_amount_needed($project_id)
{
    $amount = 0;
    
    $query = "select content_type_project.field_donation_value as field_donation_value
              from content_type_project
              where content_type_project.nid = {$project_id} ";

    $result_set = db_fetch_object(db_query($query));
    if ($result_set)
    {
       $amount = $result_set->field_donation_value;
    }

    return $amount;
}

/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_event_donation_amount_needed($event_id)
{
    $amount = 0;
    
    $query = "select content_type_event.field_event_donation_value as field_donation_value
              from content_type_event
              where content_type_event.nid = {$event_id} ";

    $result_set = db_fetch_object(db_query($query));
    if ($result_set)
    {
       $amount = $result_set->field_donation_value;
    }

    return $amount;
}
/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_project_pledge_amount($project_id)
{
    $pledge_amount = 0;
    
    $query = "select sum(content_type_make_a_pledge.field_pledge_amount_value) as field_pledge_amount
              from content_type_make_a_pledge
              inner join node on node.nid = content_type_make_a_pledge.nid
              where content_type_make_a_pledge.field_pledge_project_value = {$project_id} ";

    $result_set = db_fetch_object(db_query($query));
    if ($result_set)
    {
       $pledge_amount = $result_set->field_pledge_amount;
    }

    return $pledge_amount;
}

/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_pledged_project_of_user($user_id){
    
    $query = "select content_type_make_a_pledge.field_pledge_project_value AS project_id,node.title AS title
              from  content_type_make_a_pledge
              left join node on content_type_make_a_pledge.field_pledge_project_value = node.nid
              where node.type = 'project' AND node.uid = {$user_id} ";
    $res = db_query($query);
    
    while($result_set = db_fetch_object($res))
    {
       $project_list[$result_set->project_id] = $result_set->title;
       
    }
   if(empty($project_list)){
     $project_list['0'] = 'No Projects';
     return $project_list;
   }else{
    return $project_list;
   }
}

/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_pledged_project_and_user($user_id,$project_id){
    
    $query = "select content_type_make_a_pledge.field_pledge_project_value AS project_id,node.uid AS user_id
              from  content_type_make_a_pledge
              left join node on content_type_make_a_pledge.field_pledge_project_value = node.nid 
              where node.type = 'project' AND node.uid = {$user_id} 
              AND content_type_make_a_pledge.field_pledge_project_value = {$project_id} ";
              
    $res = db_query($query);
    
    while($result_set = db_fetch_object($res))
    {
       $project_list[$result_set->project_id] = $result_set->user_id;
       
    }
   return $project_list;
   
}

/**
 *
 * @param <type> $project_id
 * @return <type>
 */
function get_pledged_project_donors($donor_uid,$project_id){
    
    $query = "select content_type_make_a_pledge.field_pledge_project_value AS project_id,node.uid AS donor_id
              from  content_type_make_a_pledge
              left join node on content_type_make_a_pledge.nid = node.nid 
              where node.type = 'make_a_pledge' AND node.uid = {$donor_uid} 
              AND content_type_make_a_pledge.field_pledge_project_value = {$project_id} ";
              
    $res = db_query($query);
    
    while($result_set = db_fetch_object($res))
    {
       $donor_list->donor_uid = $result_set->donor_id;
       $donor_list->project_id = $result_set->project_id;
       
    }
   
    return $donor_list;
  
}


function get_project_thankyou_letter($project_id) {
    global $user;
    $thankyou_letter = array();
    
    $child_nodes = og_group_child_nids($project_id);    
    foreach($child_nodes as $key=>$val) {
        $child_node_info = node_load($val);
        if($child_node_info->type == 'project_thankyou_letter') {
            $thankyou_letter['nid'] = $child_node_info->nid;
            $thankyou_letter['text'] = $child_node_info->field_letter_body[0]['value'];
        }
    }

    return $thankyou_letter;
    
}
/**
 *
 * @param <type> $user_id
 * @return <type>
 *
 */
function _get_filtered_events($event_name = '', $state = '', $city = '', $school = '', $author = '', $name = '', $from_date = '', $to_date = '', $organization = '') {
   
   $event_name = strtoupper($event_name);
   $state = strtoupper($state);
   $city = strtoupper($city);
   $school = strtoupper($school);
   $author = strtoupper($author);
   $name = strtoupper($name);
   $organization = strtoupper($organization);
    $projects = array();  

    $join = " left join content_type_event event on event.nid = node.nid ";
            
    $join .= " Left join profile_values profile_values on profile_values.uid = node.uid ";
    $join .= " Left join profile_fields profile on profile.fid = profile_values.fid ";
    $where = " WHERE (node.type in ('event')) ";
    $order_by = " order by abs(event.field_event_donation_value) desc, event.field_event_end_time_value asc";
    
    if(!empty($event_name)) {
        
        $where .= " AND node.title LIKE \"%$event_name%\" ";
    }
    if(!empty($state)) {
        //$join .= " Left join term_node term_node on node.nid = term_node.nid";
        //$join .= " Left join term_data term_data on term_node.tid = term_data.tid";
        $where .= "AND (event.field_event_state_value = $state) ";
    }
    if(!empty($city)) {
        
        $where .= " AND event.field_event_city_value LIKE \"%$city%\" ";
    }
    if(!empty($school)) {
       
           $where .= " AND event.field_event_npo_organisation_value LIKE \"%$school%\" ";
    }
   
    if(!empty($author)) {
        $join .= " Left join users user on node.uid = user.uid";
        $where .= " AND (user.mail LIKE \"%$author%\") ";
    }
    if(!empty($name)) {
        $name = strtoupper($name);
        $where .= " AND (((profile_values.value like \"%$name%\") AND (profile.name = 'profile_first_name')) || ((profile_values.value like \"%$name%\") AND (profile.name = 'profile_contact_first_name'))) ";
    }
    //echo $from_date;
   if(is_array($from_date)){
    if(!empty($from_date['date'])) {        
       //$from_date_array = explode('-', $from_date);
      
       $from_date_timestamp = date("Y-m-d h:i:s", strtotime($from_date['date']));
       
        if(empty($to_date['date'])) {
        $to_dates = time();
        
           $to_date_timestamp = date("Y-m-d h:i:s",$to_dates);
        }
        else {
           // $to_date_array = explode('-', $to_date);
           $to_date_timestamp = date("Y-m-d h:i:s", strtotime($to_date['date']));
        }
            
        $where .= "AND event.field_event_start_time_value >= '$from_date_timestamp' and event.field_event_end_time_value <= '$to_date_timestamp' ";

    }
   }
   if(is_string($from_date)){
    if(!empty($from_date)) {        
       //$from_date_array = explode('-', $from_date);
      
       $from_date_timestamp = date("Y-m-d h:i:s", strtotime($from_date));
       
        if(empty($to_date)) {
        $to_dates = time();
        
           $to_date_timestamp = date("Y-m-d h:i:s",$to_dates);
        }
        else {
           // $to_date_array = explode('-', $to_date);
           $to_date_timestamp = date("Y-m-d h:i:s", strtotime($to_date));
        }
            
        $where .= "AND event.field_event_start_time_value >= '$from_date_timestamp' and event.field_event_end_time_value <= '$to_date_timestamp' ";

    }
   }
    

    if(!empty($organization)) {           
         $where .= "AND event.field_event_npo_organisation_value LIKE \"%$organization%\" ";      
         //$where .= "AND (profile_values.value like \"%$organization%\") AND (profile.name = 'profile_organization_name') ";

    }


    $query = "SELECT distinct(node.nid) FROM node node";
    $query .= $join;
    $query .= $where;
 //echo $query;
    $count_query = "select count(*) from ($query) count_sql";
   
    $result_set = pager_query($query,4,0,$count_query);
   
    while($row = db_fetch_object($result_set)) {
        
        $node = node_load($row->nid);
        $user = user_load($node->uid);
        profile_load_profile($user);
        
        $node_info['title'] = $node->title;
        $node_info['desc'] = $node->body;
        //$node_info['city'] = $node->field_event_place[0]['value'];
        $node_info['event_loc'] = $node->field_event_place[0]['value'];
        $node_info['end_date'] = strtotime($node->field_event_end_time[0]['value']);
        
        $node_info['donation'] = $node->field_event_donation[0]['value'];
        
        
        /*if(!empty($node->field_event_start_time[0]['value'])) {
            $node_info['event_date'] = format_date(strtotime($node->field_event_start_time[0]['value']), 'gp1');
            if(!empty($node->field_event_end_time[0]['value']) && $node->field_event_end_time[0]['value']!=$node->field_event_start_time[0]['value'] ) {
                $node_info['event_date'].=" - ".format_date(strtotime($node->field_event_end_time[0]['value']), 'gp1');
            }
        }
        else {
            $node_info['event_date'] = '';
        }*/
        
        if(!empty($node->field_event_start_time[0]['value'])) {
            $node_info['event_date'] = "From ".date("h:i a m/d/Y", strtotime($node->field_event_start_time[0]['value']))."<br/>";
            if(!empty($node->field_event_end_time[0]['value']) && $node->field_event_end_time[0]['value']!=$node->field_event_start_time[0]['value'] ) {
                $node_info['event_date'].=" To ".date("h:i a m/d/Y", strtotime($node->field_event_end_time[0]['value']));
            }
        }
        else {
            $node_info['event_date'] = '';
        }
        if(in_array('Giving Pointer',$user->roles)) {
            $node_info['author'] = l(gpapi_displayname($user), 'profile/'.$user->uid);
             $node_info['npo'] = l(gpapi_displaynponame($node), 'profile/'.$node->field_event_organization[0][value]);
             $node_info['author_uid'] = $node->field_event_organization[0][value];
        }
        if(in_array('Non-Profit Organization',$user->roles)) {
            $node_info['author'] = l($user->profile_organization_name, 'profile/'.$user->uid);
            $node_info['author_uid'] = $user->uid;
        }
      //  $node_info['author'] = $user->profile_public_name;
        $node_info['pri_image'] = theme('imagecache','113x113',$node->field_event_primary_image[0]['filepath']);
        $node_info['node_path'] = drupal_get_path_alias('node/'.$node->nid);

            $project_arr[] = $node_info;
    }

    $output['page'] = $project_arr;
    $output['pager'] = theme('pager');
    return $output;
    
}
