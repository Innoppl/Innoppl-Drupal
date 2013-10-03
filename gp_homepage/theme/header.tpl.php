<!--
<div class="float-r top-menu">
    <ul>
        <li class="first">&nbsp;</li>
        <li class="first-elevate"><a href="/about">ABOUT</a></li>
        <li><a href="/parents">FOR PARENTS</a></li>
        <li><a href="/teachers">FOR TEACHERS</a></li>
        <li><a href="/nonprofits">FOR NON-PROFITS</a></li>
        <li class="last">
            <span class="login"><a href="/user">login</a></span>
            <span class="separator">|</span>
            <span class="join"><a href="/user/register"> &nbsp;join </a></span>
        </li>
    </ul>
</div> -->
<style>
#mask,#mask3 {
  position:absolute;
  left:0;
  top:0;
  z-index:9000;
  background-color:#8C9EA8;
  display:none;
}
  
#boxes .window {
  position:absolute;
  left:0;
  top:0;
  width:440px;
  height:200px;
  display:none;
  z-index:9999;
  padding:20px;
}
#boxes3 .window3 {
  position:absolute;
  left:0;
  top:0;
  width:440px;
  height:200px;
  display:none;
  z-index:9999;
  padding:20px;
}

#boxes #dialog2 {
  background:url(http://mygivingpoint.org/sites/default/files/notice.png) no-repeat 0 0 transparent; 
  width:326px; 
  height:229px;
  padding:50px 0 20px 25px;
}
#boxes3 #dialog3 {
  background:url(http://mygivingpoint.org/sites/default/files/notice.png) no-repeat 0 0 transparent; 
  width:380px; 
  height:274px;
  padding:50px 0 20px 25px;
}

#mask4 {
  position:absolute;
  left:0;
  top:0;
  z-index:9000;
  background-color:#8C9EA8;
  display:none;
}
  

#boxes4 .window4 {
  position:absolute;
  left:0;
  top:0;
  width:440px;
  height:200px;
  display:none;
  z-index:9999;
  padding:20px;
}


#boxes4 #dialog4 {
  background:url(http://mygivingpoint.org/sites/default/files/notice.png) no-repeat 0 0 transparent; 
  width:380px; 
  height:274px;
  padding:50px 0 20px 25px;
}

</style>
<?php print $sections['top_menu']; ?>

<div class="logo">
    <a href="<?php print url(); ?>" title="<?php print t('GivingPoint'); ?>" rel="home" id="logo">
        <img src="<?php print theme_get_setting('logo'); ?>" alt="<?php print t('GivingPoint'); ?>" />
    </a>
</div>
<div class="search-box">
<div style="float:left; padding:0px 5px;">
<!-- <a href="http://dev.mygivingpoint.org/projects" target="_blank"><img src="/sites/all/themes/gptheme/images/p_icon.png" border="0"></a>
<a href="http://dev.mygivingpoint.org/events" target="_blank"><img src="/sites/all/themes/gptheme/images/event-logo.png" border="0"></a>
-->
</div> 
<div style="float:left; padding:9px 5px 0px 0px">
<?php echo $sections['search_box'];?> </div>
<a href="https://www.facebook.com/pages/GivingPoint/127294800615112" target="_blank"><img src="/sites/default/files/facebook_icon.png" border="0"></a>
<a href="https://twitter.com/#!/@givingpoint" target="_blank"><img src="/sites/default/files/twitter_icon.png" border="0"></a>
<a href="https://plus.google.com/117613311330432957976/posts" target="_blank"><img src="/sites/default/files/Google_Plus_icon.png" border="0"></a>
</div>

<?php if(!empty($nav_tree['pri_nav'])):?>
<div class="hover-menu clear sharp-border">
    <ul>
    <?php foreach($nav_tree['pri_nav'] as $term):
            if(drupal_get_path_alias("nav/".$term->tid) == 'nav/'.$term->tid):?>
        <li>
            <a href="<?php echo base_path(). "{$term->name}";?>"><span class='top'><?php echo $term->fields['display_name'];?></span><span class='bottom'><?php echo $term->fields['tagline'];?></span></a>
        </li>
            <?php else :?>
        <li>
            <a href="<?php echo base_path(). drupal_get_path_alias("nav/".$term->tid);?>"><span class='top'><?php echo $term->fields['display_name'];?></span><span class='bottom'><?php echo $term->fields['tagline'];?></span></a>
        </li>
        <?php endif; ?>
    <?php endforeach; ?>
    </ul>
    <div class="clear"></div>
</div>
<?php endif;?>

<?php if(!empty($nav_tree['sec_nav'])):?>
<div class="hover-secondary-menu clear sharp-border">
    <ul>
    <?php foreach($nav_tree['sec_nav'] as $term): ?>
    <?php if(($term->name != 'Giving Guidance') && ($term->name != 'Volunteer Guidance') && ($term->name != "Today's Hot Issues") && ($term->name != "Find Your Passion")): ?>
        <li>
            <a class="head-first" href="<?php echo drupal_get_path_alias("nav/".$term->tid);?>"><?php echo $term->name;?></a>
        </li>
        <?php elseif($term->name == "Today's Hot Issues"): ?>
	<li>
		<a class="head-first" href="/todays-hot-issues"><?php echo $term->name;?></a>
	</li>
        <?php else: ?>
        <?php $path = strtolower(str_replace(array(" ","'"), array("-",""), $term->name));?>
        <li>
            <a class="head-first" href="<?php echo "/causes/{$path}";?>"><?php echo $term->name;?></a>
        </li> 
        <?php endif; ?>
    <?php endforeach; ?>
    </ul>
</div>
<?php endif;?>

<div id="boxes3">                  
<!-- Start of Sticky Note -->
<div id="dialog3" class="window3"><br/>
  <center><b>What would you like to do?</b></center><br/>Raise funds for a community project, mission trip, or cause  <br/><center><a href="http://mygivingpoint.org/node/add/project"><img src="http://mygivingpoint.org/sites/default/files/projects.png"></a></center><br/><br/>
  Post a one time volunteer opportunity or a special event <br/><a href="http://mygivingpoint.org/node/add/event"><center><img src="http://mygivingpoint.org/sites/default/files/events.png"></a></center> <br/><br/>

</div>
<!-- End of Sticky Note -->



<!-- Mask to cover the whole screen -->
  <div id="mask3"></div>
</div>


<div id="boxes4">                  
<!-- Start of Sticky Note -->
<div id="dialog4" class="window4"><br/>
  <center><b>What would you like to do?</b></center><br/>Raise funds for a community project, mission trip, or cause  <br/><center><a href="http://mygivingpoint.org/node/add/project"><img src="http://mygivingpoint.org/sites/default/files/projects.png"></a></center><br/><br/>
  Post a one time volunteer opportunity or a special event <br/><a href="http://mygivingpoint.org/node/add/event"><center><img src="http://mygivingpoint.org/sites/default/files/events.png"></a></center> <br/><br/>

</div>
<!-- End of Sticky Note -->



<!-- Mask to cover the whole screen -->
  <div id="mask4"></div>
</div>