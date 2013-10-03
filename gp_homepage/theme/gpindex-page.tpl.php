<?php
/**
 * This is the main template for the index page of mygivingpoint.org
 *
 * @package givingpoint_package
 * @subpackage gp_homepage
 *
 * @category module
 *
 * $sections: An array which contains the html to be rendered for diffrent sections of the page.
 */
?>
<style>
.reveal-modal-bg { 
		position: fixed; 
		height: 100%;
		width: 100%;
		background: #000;
		background: rgba(0,0,0,.8);
		z-index: 100;
		display: none;
		top: 0;
		left: 0; 
		}
	
.reveal-modal {
		visibility: visible;
		top: 100px; 
		left: 50%;
		margin-left: -300px;
		width: 520px;
		height: 250px;
		background: #eee url(../sites/default/files/modal-gloss.png) no-repeat -200px -80px;
		position: absolute;
		z-index: 101;
		padding: 30px 40px 34px;
		-moz-border-radius: 5px;
		-webkit-border-radius: 5px;
		border-radius: 5px;
		-moz-box-shadow: 0 0 10px rgba(0,0,0,.4);
		-webkit-box-shadow: 0 0 10px rgba(0,0,0,.4);
		-box-shadow: 0 0 10px rgba(0,0,0,.4);
		}
		
	.reveal-modal.small 		{ width: 200px; margin-left: -140px;}
	.reveal-modal.medium 		{ width: 400px; margin-left: -240px;}
	.reveal-modal.large 		{ width: 600px; margin-left: -340px;}
	.reveal-modal.xlarge 		{ width: 800px; margin-left: -440px;}
	
	.reveal-modal .close{
		font-size: 22px;
		line-height: .5;
		position: absolute;
		top: 8px;
		right: 11px;
		color: #aaa;
		text-shadow: 0 -1px 1px rbga(0,0,0,.6);
		font-weight: bold;
		cursor: pointer;
		}
	.reveal-modal.large p{
		font-size: 14px;
		font-family: Arial, Verdana, Georgia, sans-serif;
		
	}
#mask5{
  position:absolute;
  left:0;
  top:0;
  z-index:9000;
  background-color:#8C9EA8;
  display:none;
}
  

#boxes5 .window5{
  position:absolute;
  left:0;
  top:0;
  width:440px;
  height:200px;
  display:none;
  z-index:9999;
  padding:20px;
}



 
</style>
<div class="gp-homepage-main">
    <div class="gp-homepage-level1">
        <div class="gp-homepage-slideshow float-l"><?php echo $sections['slideshow'];?></div>
        <div class="gp-homepage-description float-r"><?php echo $sections['desc_right'];?></div>
    </div>
    <div class="gp-homepage-level2 clear">
        <div class="home-page-block gp-homepage-left"><?php echo $sections['newsblock'];?></div>
        <div class="home-page-block gp-homepage-middle"><?php echo $sections['desc_middle'];?></div>
        <div class="home-page-block gp-homepage-right center-align">
        <!-- <div class="decore"><a href="/donors-learn-more" class="donate-button-gp">Donate Now!</a></div> -->
        <?php echo $sections['newsletter'];?>
        </div>
    </div>
</div>
<div id="boxes5">                  
<!-- Start of Sticky Note -->
<div id="dialog5" class="window5">
  <div id="myModal" class="reveal-modal large"><br/><br/><br/>
			<h1><font color="#1993CC">We've done it! Our students have volunteered 150,000 hours in just three years!</font></h1><br/>
			<br/>
			<p> Congratulations, <b>Samir</b> from Alpharetta, GA and all other students who've helped log more than 150,000 community service hours through GivingPoint!</p>
			<a class="close">&#215;</a>
</div>
</div>
<!-- End of Sticky Note -->



<!-- Mask to cover the whole screen -->
  <div id="mask5"></div>
</div>
