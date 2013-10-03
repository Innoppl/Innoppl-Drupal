<div class="footer-sponsor" align="center"><span>Our Sponsors</span>
<a href="http://newkentcap.com/"><img src="sites/all/themes/gptheme/images/NewKent.png" width="100px" height="100px"></a>
<a href="http://statefarm.com/"><img src="sites/all/themes/gptheme/images/State_Farm.png" width="100px" height="100px"></a>
<a href="http://suntrust.com/"><img src="sites/all/themes/gptheme/images/SunTrust logo.png" width="100px" height="100px"></a>
</div>
<div class="clear"></div>
<div class="footer-head">
<span>get involved. give back.</span>
</div>

<?php $links1 = module_invoke('gpapi', 'get_nav_tree');  ?>
<div class="footer-body">
    <ul>
         <?php $ctr=0; ?>
        <?php foreach($links1['pri_nav'] as $term): ?>
         <?php $ctr++; ?>
        <li>
            
            <a  <?php if($ctr==5):?> style="border-right:none;" <?php endif;?>  href="<?php echo 'http://'.$_SERVER['HTTP_HOST'].'/'.drupal_get_path_alias("nav/".$term->tid);?>">
            <?php echo $term->name;?>&nbsp;<?php echo $term->fields['tagline'];?>
            </a>

        </li>
        <?php endforeach; ?>
    </ul>


    <?php $links2 = menu_secondary_links();
    echo"<pr>";

    ?>
    <ul>
        <?php foreach($links2 as $key=>$link):?>
            <li class="links2">
            <?php echo l($link['title'], $link['href']);?>
            </li>
        <?php endforeach;?>

        <li class="links2">
            <a href="/terms-conditions">Terms & Conditions</a>
        </li>
        <li class="links2">
            <a href="/privacy-policy">Privacy Policy</a>
        </li>
        <li class="links2">
            <a style="border-right:none;" href="/contact-us">Contact Us</a>
        </li>

    </ul>
    <span>COPYRIGHT 2009 GIVINGPOINT.&nbsp;ALL RIGHTS RESERVED.</span>
    <span>Designed, Developed & Maintained by <a href="http://www.innoppl.com">Innoppl</a></span>
</div>