<?php

// 表示するだけだと PHP でやってる意味があまりないので、1
// ファイル画像として保存してみた。

require_once('HSVtoRGB.php');

$width = 300;
$height = 300;
$button_list = array();

function display() {
    global $button_list;
    global $width, $height;

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
        
    foreach ($button_list as $button) {
        glPushMatrix();
        $color = array($button['red'], $button['green'], $button['blue'], 0.9);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, $color);
        glTranslatef($button['x'], $button['y'] , 1.0);
        glutSolidSphere(0.15, 16, 10);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glFlush();

    // OpenGL がレンダリングした画像pixel を吸い出す
    // red, green blue の順で並ぶ > GL_RGB

    $pixels = glReadPixels_yoya(0, 0, $width, $height, GL_RGB);
    // glReadPixels binding のバグ取り間に合わなかっので、
    // 独自に関数作ってみた。後でなんとかします。。。

    // GD でキャンバスを作る
    $im = imagecreatetruecolor($width, $height);
    $i = 0;
    // pixels 配列を R, G, B の順で解釈してイメージを作成
    for ($y = $height ; $y >= 0; $y--) {
    	for ($x = 0; $x < $width ; $x++) {
	    $red   = ord($pixels[$i]);
	    $green = ord($pixels[$i + 1]);
	    $blue  = ord($pixels[$i + 2]);
	    $color = imagecolorallocate($im, $red, $green, $blue);
	    imagesetpixel($im, $x, $y, $color); // pixel を埋めていく
	    $i += 3;
   	}
	// 横scan 1行は 4 の倍数になるよう padding されるので、その補正
	// Windows BMP もそうだし、これは結構一般的な仕様。
	if (($i % 4) != 0) {
	    $i += 4 - ($i % 4); // 4 の倍数に足りない分 increment
	}
    }
    imagepng($im, "output.png"); // PNG ファイルとして保存!!!
}

function keyboard($key, $x, $y) {
    if ($key == chr(0x1b)) { // Esc key
        exit(0);
    }
}

function special($key, $x, $y) {
    $delta_x = 0.03;
    $delta_y = 0.03;
    switch ($key) {
    case  GLUT_KEY_LEFT:
        glTranslatef(-$delta_x, 0, 0.0);
        break;
    case  GLUT_KEY_RIGHT:
        glTranslatef($delta_x, 0, 0.0);
        break;
    case  GLUT_KEY_UP:
        glTranslatef(0, $delta_y, 0.0);
        break;
    case  GLUT_KEY_DOWN:
        glTranslatef(0, -$delta_y, 0.0);
        break;
    case  GLUT_KEY_HOME:
        glRotatef(10, 0, 1, 0);
        break;
    case  GLUT_KEY_END:
        glRotatef(-10, 0, 1, 0);
        break;
    }
    glutPostRedisplay();
}

function mouse($button, $state, $x, $y) {
    if ($state != 0) {
        return ;
    }
    echo "mouse: $button $state: ($x, $y)\n";
    glutPostRedisplay();
}

function init() {
    global $argv;
    global $width, $height;
    glutInit($argv);
    
//    glutInitWindowSize(400, 400);
    glutInitWindowSize($width, $height);

//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
        
    glutCreateWindow("Color Button");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    $lit_amb = array(1.0, 1.0, 1.0, 0.0); /* 環境光の強さ */
    $lit_dif = array(1.0, 1.0, 1.0, 0.0); /* 拡散光の強さ */
    $lit_spc = array(1.0, 1.0, 1.0, 0.0); /* 鏡面反射光の強さ */
    $lit_pos = array(-5.0, 5.0, -8.0, 1.0);  /* 光源の位置 */
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, $lit_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, $lit_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, $lit_spc);
    glLightfv(GL_LIGHT0, GL_POSITION, $lit_pos);
    
    /* ライトを有効にする */
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glDisable(GL_DEPTH_TEST) ;

    glBlendFunc(GL_ONE, GL_ONE);
}

$button_list = array(
    array('c' => '#'), array('c' => '0'), array('c' => '*'),
);

for ($i = 1 ; $i <= 9 ; $i++) {
    $button_list[] = array('c' => (string)$i);
}

foreach ($button_list as $idx => &$button) {
    $button['x'] = -0.25 + 0.22 * ($idx % 3);
    $button['y'] = -0.35 + 0.22 * floor(($idx / 3));
    $hsv = array((360 / 12) * $idx, 160, 255);
    $rgb = HSVtoRGB($hsv);
    $button['red']   = $rgb[0] / 256;
    $button['green'] = $rgb[1] / 256;
    $button['blue']  = $rgb[2] / 256;
}

init();

glutDisplayFunc('display');
// display(); exit(0);

glutKeyboardFunc('keyboard');
glutSpecialFunc('special');
glutMouseFunc('mouse');

glutMainLoop();
