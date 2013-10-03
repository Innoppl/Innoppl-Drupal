<?php if(isset($_GET['reciept'])):?>
<div><?php echo l("<img src = 'sites/default/files/gptheme_logo.jpg'>",'',array('html'=>true))?></div> <br />
 <br /> <br />
<h3>Giving Card Details </h3>
<div><strong>Amount:</strong>&nbsp;<?php echo "$".number_format($node->field_givingcode_amount[0]['value'],2);;?></div>
<div><strong>Gift card code:</strong>&nbsp;<?php echo $node->field_givingcode[0]['value'];?></div>
<div><strong>From: </strong>&nbsp;<?php echo $node->field_from_name[0]['value'];?></div>
<div><strong>To: </strong>&nbsp;<?php echo $node->field_to_name[0]['value']?></div>
<div><strong>Expiration: </strong>&nbsp;<?php echo "1 year";?></div>
<div><strong>GivingPoint Tax ID: </strong>&nbsp;27-0428207</div>
 
<?php 
    $workflow = workflow_get_state($node->_workflow);
?>
 <div><strong>Status: </strong>&nbsp;<?php echo $workflow['state']; ?></div>

<p>Support a Teen at <small><?php echo l('www.mygivingpoint.org','');?></small>
<?php else: ?>
<br />
<div>Thank you for taking the time to help a young social entrepreneur on GivingPoint.  Welcome to our movement to unleash the passion and energy of young people to create a more connected compassionate world! Your support will fund a project that has been posted by a young civic philanthropist. It's easy to use and fun to watch your investment change lives! </div>
<div class = '' style="background:transparent url(/sites/all/themes/gptheme/images/givingcard-noshadow.gif) no-repeat scroll 0 0;height:213px;padding-left:15px;padding-top:9px;font-family:'trebuchet MS',Verdana;font-size:14px;font-weight:bold;line-height:30px;text-shadow:1px 1px #F5F5F5;width:357px;margin-top:12px;">

</div>
<div style="width:557px;">
<p style='width:318px;margin:0;padding-bottom:5px;'><b>From:</b> <?php echo $node->field_from_name[0]['value'];?></p>
<?php if($node->field_to_name[0]['value']!=""):?>
<p style='width:318px;margin:0;padding-bottom:15px;'><b>To:</b> <?php echo $node->field_to_name[0]['value'];?></p>
<?php endif; ?>
<p style='width:318px;margin:0;padding-bottom:5px;'><b>Amount:</b> <?php echo "$".number_format($node->field_givingcode_amount[0]['value'],2);?></p>
<p style='width:318px;margin:0;padding-bottom:15px;'><b>Redemption Code:</b> <?php echo $node->field_givingcode[0]['value'];?></p>

<p style='width:318px;margin:0;padding-bottom:8px;'><b>Expires in:</b> 1 year</p>
<p style='width:518px;margin:0;padding-bottom:5px;font-size:1.2em;font-weight:bold;'>Redeem at http://www.mygivingpoint.org/redeem</p>
</div>

<?php endif;?>