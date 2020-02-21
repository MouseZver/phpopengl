<?php

function rgb2hsv( $R, $G, $B )
{
	$R /= 255;
	$G /= 255;
	$B /= 255;

	$maxRGB = max ( $R, $G, $B );
	$minRGB = min ( $R, $G, $B );
	$chroma = $maxRGB - $minRGB;

	$resV = round ( 100 * $maxRGB );

	if ( $chroma == 0 )
	{
		return [ 0, 0, $resV ];
	}

	if ( $R == $minRGB )
	{
		$h = 3 - ( ( $G - $B ) / $chroma );
	} 
	elseif ( $B == $minRGB )
	{
		$h = 1 - ( ( $R - $G ) / $chroma );
	}
	else
	{
		$h = 5 - ( ( $B - $R ) / $chroma );
	}
		
		
	return [ 
		round ( 60 * $h ), 
		round ( 100 * ( $chroma / $maxRGB ) ), 
		$resV
	];
}



function hsv2rgb( $H, $S, $V )
{
	if ( $S == 0 )
	{
		$c = round ( 256 * $V / 100 );
		
		return [ $c, $c, $c ];
	}
	
	$i = floor ( ( $H / 60 ) % 6 );
	
	$min = ( ( 100 - $S ) * $V ) / 100;
	
	$a = ( $V - $min ) * ( ( $H % 60 ) / 60 );
	
	$inc = $min + $a;
	
	$dec = $V - $a;
	
	[ $R, $G, $B ] = [
		0 => static function ( $v, $m, $i, $d ): array { return [ $v, $i, $m ]; },
		1 => static function ( $v, $m, $i, $d ): array { return [ $d, $v, $m ]; },
		2 => static function ( $v, $m, $i, $d ): array { return [ $m, $v, $i ]; },
		3 => static function ( $v, $m, $i, $d ): array { return [ $m, $d, $v ]; },
		4 => static function ( $v, $m, $i, $d ): array { return [ $i, $m, $v ]; },
		5 => static function ( $v, $m, $i, $d ): array { return [ $v, $m, $d ]; },
	][$i]( 
		$V,
		$min, 
		$inc,
		$dec,
	);
	
	return [
		round ( 255 / 100 * $R ),
		round ( 255 / 100 * $G ),
		round ( 255 / 100 * $B ),
	];
}
