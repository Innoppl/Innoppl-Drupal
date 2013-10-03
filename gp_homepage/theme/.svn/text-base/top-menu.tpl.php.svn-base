<?php global $user; ?>
<div class="float-r top-menu">
    <ul>
        <li class="first">&nbsp;</li>
        <?php $i=1; foreach($links as $key=>$link):?>
        <li <?php if($i==1):?> class="menu-start" <?php endif; ?>><a href="<?php echo $link['href'];?>"><?php echo $link['title'];?></a></li>
        <?php $i++;  endforeach;?>
        <li class="last">
            <?php if(empty($user->uid)): ?>
                <span class="login"><a href="/user">login</a></span>
                <span class="separator">|</span>
                <span class="join"><a href="/user/register">join</a></span>
            <?php else:?>
            <span class="dashboard"><a href="/<?php echo drupal_get_path_alias("profile/".$user->uid); ?>">My Profile</a></span>
                <span class="logout"><a href="/logout">logout</a></span>
            <?php endif; ?>
        </li>
    </ul>
</div>