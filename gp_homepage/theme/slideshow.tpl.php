<link type="text/css" href="<?php echo base_path();?>sites/all/modules/givingpoint/gp_homepage/theme/bottom.css" rel="stylesheet" />

<div id="slideshow-wrapper" style='background-color:#F5F5F5;'>
    <div id="hidewrapper" style='display:none' >
        <div id="slides">

            <div id='image' style='height:216px;'>
                <?php
                $node_i=0;
                foreach($nodes as $node):
                ?>
                <!--<a href='<?php if(stristr($node->field_slide_link[0]['url'],'http')) echo $node->field_slide_link[0]['url']; else echo 'http://'.$node->field_slide_link[0]['url'];?>'><img src="<?php echo base_path().$node->field_slide_image[0]['filepath'];?>" ref='<?php echo $node_i;?>' reftext='<?php echo $node->field_slide_subtext[0]['value'];?>'/></a> -->
                <?php echo l(theme('imagecache', '542x216', $node->field_slide_image[0]['filepath'], '', '', array('ref'=>$node_i, 'reftext'=>$node->field_slide_subtext[0]['value'])), $node->field_slide_link[0]['url'] ,
                              array('html'=>true));?>
                <?php
                $node_i++;
                endforeach;
                ?>
            </div>
            <div id='thumbnail'>
                <ul id="pikame">
                    <?php
                    $node_i=0;
                    foreach($nodes as $node):
                    ?>
                    <li >
                        <!-- <img src="<?php echo base_path().$node->field_slide_thumb[0]['filepath'];?>" ref='<?php echo $node_i;?>'/> -->
                        <?php echo theme('imagecache', '126x60', $node->field_slide_image[0]['filepath'], '', '', array('ref'=>$node_i));?>
                    </li>
                    <?php
                    $node_i++;
                    endforeach;
                    ?>
                </ul>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript" src="<?php echo base_path();?>sites/all/modules/givingpoint/gp_homepage/theme/slideshow.js"></script>
