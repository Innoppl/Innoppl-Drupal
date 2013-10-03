
var $slideshow_cycle_id;
var $slideshow_timeout=4000;
			
			
function slideSwitch($currentSlide) {
	//execute slideshow once last image loads up
	$thumbnails=$('#thumbnail IMG');
	$images=$('#image IMG');

	for(i=0;i<4;i++)
	{
		if(!($thumbnails[i].complete && $images[i].complete))
		{
			return;
		}
	}
	//after initialization,set interval to transition time
	if($currentSlide==-2){
		clearInterval($slideshow_cycle_id);
		$slideshow_cycle_id=setInterval( "slideSwitch(-1)", $slideshow_timeout );
		$('#hidewrapper').css('display','block');
		$('#image IMG:first').addClass('active');
		$('#thumbnail IMG:first').addClass('thumbactive');
		$('#thumbnail IMG:first').siblings().addClass('pika_div_selected');
		return;
	}
    var $active = $('#thumbnail IMG.thumbactive');

    if ( $active.length == 0 ) {
        $active = $('#thumbnail IMG:first');
    }
	$active_index=$active.attr('ref');
	$images.stop(true,true);
    $t=parseInt($active_index);
    $active_image=$images.eq($t);
   
    if($currentSlide!=-1)
    {
        $active_index=$currentSlide!=0?$currentSlide-1:4;
        clearInterval($slideshow_cycle_id);
        $slideshow_cycle_id=setInterval( "slideSwitch(-1)", $slideshow_timeout );
    }

    $t=parseInt($active_index);

    if(($t)>=($images.length-1))
    {
        $next_image=$images.eq(0);
        $next=$thumbnails.eq(0);
    }
    else
    {
        $next_image=$images.eq($t+1);
        $next=$thumbnails.eq($t+1);
    }
	//if the next image is same as old one, do nothing
	if($active.attr('ref')==$next.attr('ref'))
	{
		return;
	}
    $active_image.addClass('last-active');
	$thumbnails.removeClass('thumbactive');
	$next.addClass('thumbactive');
	
	$next.siblings().addClass('pika_div_selected');
	$active.siblings().removeClass('pika_div_selected');
	
    $next_image.css({opacity: 0.0})
        .addClass('active')
        .animate({opacity: 1.0}, 1000, function() {
            $active_image.removeClass('active last-active');
        });
}



$('#pikame  img').click(
    function(){
        slideSwitch($(this).attr('ref'));
    }
);

$('#pikame  img').hover(
    function(){
        $(this).addClass('thumbhover');
        $(this).siblings().addClass('pika_div_hover');
    },
    function(){
        $(this).removeClass('thumbhover');
        $(this).siblings().removeClass('pika_div_hover');
    }
);


$('#pikame > li > div').hover(
    function(){
        $(this).addClass('pika_div_hover');
        $(this).siblings().addClass('thumbhover');
    },
    function(){
        $(this).removeClass('pika_div_hover');
        $(this).siblings().removeClass('thumbhover');
    }
);



$('#image').hover(
    function(){
        clearInterval($slideshow_cycle_id);
    },
    function(){
        $slideshow_cycle_id=setInterval( "slideSwitch(-1)", $slideshow_timeout );
    }
);

$slideshow_cycle_id=setInterval( "slideSwitch(-2)",500);