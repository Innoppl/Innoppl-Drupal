<?php global $theme; $theme_path = drupal_get_path("theme", $theme);?>
<div class="usergrid ">
    <table>
        <tbody>
        <?php $project_info = node_load(arg(1));?>
            <?php foreach ($table as $tr_id => $tr):?>
             <tr><td colspan="8" style="padding:1px;"></td></tr>
            <tr class="tr-<?php echo $tr_id; ?>">
                <?php foreach($tr as $td_id=>$td ):?>
                <td style="padding:1px;"></td>
                <td class="td-<?php echo $tr_id.'-'.$td_id?> user-box">
                    <div>
                    <?php if(isset($td->could_be_you) && $td->could_be_you):?>
                    <?php echo l(theme('imagecache', '56x56', $td->picture), $td->link, array('html'=>TRUE,  'query'=>array('ref'=>$td->ref)));?>
                    <?php else: ?>
                    <?php if(in_array('Givingpoint Donor', $td->roles)) :
                             echo l(theme('imagecache', '56x56', "$theme_path/images/155_blog.png"), ("profile/{$project_info->uid}"), array('html'=>TRUE,));?>
                             <div class='grid-anchor-w'><?php echo l("GivingPoint grant", ("profile/{$project_info->uid}"), array('html'=>TRUE));?></div>
                             <div>
                             <img src='/sites/all/themes/gptheme/images/star-donate.png'  alt='GivingPoint Donor' title='GivingPoint Donor' style='left:25px;position:relative;top:-104px;'/>
                             </div>
                               <?php     
                                 ?>

                    <?php else :?>
                        <?php if(!gpapi_get_userpic($td->uid)):?>
                            <?php echo l(theme('imagecache', '56x56', "$theme_path/images/165_blog.png"), ("profile/{$td->uid}"), array('html'=>TRUE));?>
                        <?php else: ?>
                            <?php if($td->uid !='anonymous'): echo l(theme('imagecache', '56x56', gpapi_get_userpic($td->uid)), ("profile/{$td->uid}"), array('html'=>TRUE,)); else: echo l(theme('imagecache', '56x56', "$theme_path/images/gp_star_pic.png"), ("profile/{$td->uid}"), array('html'=>TRUE)); ?>
                        <?php endif; ?>
                        <?php endif; ?>
                            <div class='grid-anchor-w'><?php if($td->uid == 0){ echo "<font size='4' color='#85B1CA'>GivingPoint</font>"; if($td->donation !=''){ echo "<br/><font size='4' color='#85B1CA'>$". $td->donation ."</font>"; } } elseif($td->uid !='anonymous'){ echo l(gpapi_displayname($td), ("profile/{$td->uid}"), array('html'=>TRUE)); if($td->donation !=''){ echo "<br/><font size='4' color='#85B1CA'>$".$td->donation."</font>"; } } else{ echo "<font size='4' color='#85B1CA'>Anonymous</font>"; if($td->donation !=''){ echo "<br/><font size='4' color='#85B1CA'>$".$td->donation."</font>"; } } ?></div>
                        
                        <?php endif; ?>                 

                    <?php endif; ?> 
                    </div>
                </td>
                <td style="padding:1px;"></td>
                <?php endforeach; ?>
            </tr>
            <tr><td colspan="8" style="padding:1px;"></td></tr>
            <?php endforeach; ?>
        </tbody>
    </table>
</div>
