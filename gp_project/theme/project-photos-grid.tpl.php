<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

?>

<div class = 'entry-heading'><?php echo $heading; ?></div>
<div class="entry-block-wrapper">

    <?php  if(isset($sections['images']) && !empty($sections['images'])):
     $table = array_chunk($sections['images'],4);?>
    <table>
        <tbody>
            <tr> <td class="border-b-thin">
            <table><tbody>
           <?php  foreach($table as $table_key => $table_val) : ?>
            <tr>
           <?php foreach($table_val as $image) : ?>
        <?php //echo "<pre>";print_r($sections['config']);?>

                    <td class="user-info-blocks center-align border-b-medium">
                        <div class = "user-entry-image ">                            
                                <?php echo theme('imagecache',$sections['config']['image_size']['thumbnail'],$image->filepath); ?>
                        </div>
                        <!--<div class = "user-entry-desc"><?php echo $image->title; ?></div>-->
                    </td>


            <?php endforeach;?> 
            </tr>
            <?php endforeach;?>
            </tbody></table>
             </td>
             </tr>

        </tbody>
    </table>

    <?php else:?>
    <div class="center-align"><h3 class='box-inline'>No Entries</h3></div>
    <?php endif; ?>


</div>
