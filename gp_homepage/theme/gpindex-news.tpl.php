<?php
/**
 * This is the news section for the index page of mygivingpoint.org
 *
 * @package givingpoint_package
 * @subpackage gp_homepage
 * @author aradhana <aradhana@axelerant.com>
 * @category module
 *
 * $sections: An array which contains the data to be rendered for news section of the page.
 */#E2EBD0
?>
<style>
.smain{
margin-left:3px;
}
.stop{		
background:url("http://www.mygivingpoint.org/sites/all/themes/gptheme/images/bdr_widget_220.gif") no-repeat scroll center top #E2EBD0;
margin:0;
padding-top:7px;
width:220px;

}
.sbottom{		
background:url("http://www.mygivingpoint.org/sites/all/themes/gptheme/images/bdr_widget_220.gif") no-repeat scroll center bottom #E2EBD0;
margin:0;
padding-top:7px;
width:220px;
}
.smiddle{
	background-color:#FFF;
	width:212px;
	line-height: 22px;
	padding:4px;
}
</style>

<center><div class="smain">
<div class="stop"></div>
<div class="smiddle"><h1 style="font-family:verdana; align:center; color:#A7C9DA; font-weight:bold; text-decoration:underline;">VIDEO WALL</h1><br/>
<center><a target="_blank" href="givingpoint-videos">Get Inspired by Viewing cause videos</a></center>
</div>
<div class="sbottom"></div>
</div></center>
<h2></h2>
<div style='overflow:hidden;'>
<?php if(!empty($sections)):?>
    <?php foreach($sections as $entry){ ?>
    <p>
        <a href="/<?php echo $entry['node_path'];?> ">
            <?php echo $entry['title'];?>
        </a>

        <?php echo $entry['creation_date'] ?>
    </p>
    <?php } ?>
    <?php echo l('See all news &raquo','allnews', array('html' => true, attributes => array('class'=>'font-weight-b box-block text-r'))); ?>
</div>

<?php endif; ?>
