<div id="list-blogs-block">
    <ul>
        <?php foreach($items as $node):?>
        <li><?php echo l($node->title, $node->path);?></li>
        <?php endforeach; ?>
    </ul>
</div>