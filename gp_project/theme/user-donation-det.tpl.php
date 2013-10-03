<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $
?>


<?php 
foreach($project_assoc as $key => $val) { 
    $project_info = node_load($key);
    $donor = user_load($section->uid);
}

$donor_name = gpapi_displayname($donor);
if(isset($project_info->field_organization[0]['value']) && !empty($project_info->field_organization[0]['value']) && ($project_info->field_organization[0]['value'] != 0)) {    
    $npo_info  = user_load($project_info->field_organization[0]['value']);
    profile_load_profile($npo_info);
    $benefiting_org = $npo_info->profile_organization_name;
}
elseif(($project_info->field_organization[0]['value'] == 0) && (!empty($project_info->field_non_profit_organisation[0]['value']))) {
    $benefiting_org = $project_info->field_non_profit_organisation[0]['value'];
}
else {    
    $npo_info = user_load($project_info->uid);
    profile_load_profile($npo_info);
    $benefiting_org = $npo_info->profile_organization_name;
}
if(module_exists('phonetic')) {
    if(!isset($project_info->temp_project_title)) {
        $project_info->temp_project_title = $project_info->title;
        $project_info->title = phonetic_filter_process($project_info->temp_project_title);
    }
    if(!isset($project_info->temp_project_desc)) {
        $project_info->temp_project_desc = $project_info->og_description;
        $project_info->og_description = phonetic_filter_process($project_info->temp_project_desc);
    }
}

if($section->field_donation_type[0]['value'] == 'Giving Card')
    $donation_type = "Giving Card";
elseif($section->field_donation_type[0]['value'] == 'By Check') {
    $donation_type = "By Check";
}
else{
    $donation_type = "Paypal";
}


?>
<div><?php echo l("<img src = 'sites/default/files/gptheme_logo.jpg'>",'',array('html'=>true))?></div> <br />
 <br />
<h3 class="blue">Project&nbsp;Donation&nbsp;Details </h3><div class = 'clear'></div> <br />

<div><strong>Donation Id:</strong>&nbsp;<?php echo $section->nid;?></div>
<br/>
<div><strong>Date of Donation:</strong>&nbsp;<?php echo format_date($section->created, 'gp1');?></div>
<br/>
<div><strong>Benefiting Organization:</strong>&nbsp;<?php echo $benefiting_org;?></div>
<br/>
<div><strong>Project Name: </strong>&nbsp;<?php echo $project_info->title;?></div>
<br/>
<div><strong>Project Description: </strong>&nbsp;<?php echo $project_info->og_description;?></div>
<br/>
<div><strong>Donation amount: </strong>&nbsp;<?php echo "$".number_format($section->field_donation_amount[0]['value'],2);?></div>
<br/>
<?php if(!empty($section->field_payment_details[0]['value'])):?>
<div><strong>Payment Details: </strong>&nbsp;<?php echo $section->field_payment_details[0]['value'];?></div>
<br/>
<?php endif;?>
<div><strong>Donation type: </strong>&nbsp;<?php echo $donation_type ;?></div>
<br/><br/>
<p>GivingPoint is forever grateful to our generous sponsors and supporters.
We have one thing in common. We are passionate about creating a new generation of concerned young citizens who thrive while improving their community through volunteer service and philanthropy.  Thank you for your donation to help create a world of civic leaders, one teenager at a time!
<br/>
<br/>
GivingPoint is a 501(c)(3) not-for-profit corporation.
<br/>
<br/>
Tax ID number:  27-0428207.
</p>
