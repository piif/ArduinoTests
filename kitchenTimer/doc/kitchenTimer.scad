/**
  debug :
    % ... -> affiche en transparence, mais l'objet n'est PAS pris en compte dans la suite
    # ... -> affiche en transparence, mais l'objet EST pris en compte dans la suite
    ! ... -> on ne prend en compte que ce bloc
    * ... -> on ignore ce bloc (comme s'il était en commentaire)
**/

// A CREUSER : https://github.com/openscad/openscad/issues/301
// resize() ?
// $xmax... après un render() ?

$fn=100;

use <tools.scad>


// Convention for a component name "xyz" :
// - module "xyz" renders component with X & Y centered, bottom at Z=0
// - variable "xyz_size" contains component bounding box with X & Y centered, bottom at Z=0
// - variables "xyz_something_size" may contain sub components bounding box
// the idea is to externalize all dimensionqs from rendering modules, to make them available for further computations

batteryAAA_size = [ 10.3, 10.3, 44 ];
batteryAAA_pin_size = [ 4, 4, 1.5 ];
module batteryAAA(bbox=false) {
    color("brown") union() {
        cylinder(d=batteryAAA_pin_size.x, h=batteryAAA_size.z);
        cylinder(d=batteryAAA_size.x, h=batteryAAA_size.z - batteryAAA_pin_size.z);
    }
    if (bbox) {
        color("gold", 0.5) Z0cube(batteryAAA_size);
    }
}

screen_size = [ 38, 19, 7 ];
module screen(bbox=false) {
    color("white") Z0cube(screen_size);
    if (bbox) {
        color("gold", 0.5) Z0cube(screen_size);
    }
}

button_size = [ 6, 6, 7 ];
button_top_size = [ 3.3, 3.3, 3 ];
module button(bbox=false) {
    bottom = button_size.z - button_top_size.z;
    union() {
        color("silver") Z0cube([ button_size.x, button_size.y, bottom ]);
        color("black") cylinder(d=button_top_size.x, h=button_size.z);
    }
    if (bbox) {
        color("gold", 0.5) Z0cube(button_size);
    }
}

nano_plate_size = [ 43, 17.5, 1.5 ];
nano_usb_plug_size = [ 9.5, 7.5, 4 ];
nano_usb_plug_delta = 2;
nano_ICSP_plug_size = [ 5, 8, 9 ];
nano_size = [ nano_plate_size.x+nano_usb_plug_delta, nano_plate_size.y, nano_plate_size.z + max(nano_usb_plug_size.z, nano_ICSP_plug_size.z) ];
module nano(bbox=false) {
    union() {
        // plate
        color("blue") translate([ nano_usb_plug_delta/2, 0, 0]) Z0cube(nano_plate_size);
        // usb plug
        color("silver")
            // USB plug on left of plate, but outside of nano_usb_plug_delta
            translate([ (nano_usb_plug_size.x-nano_plate_size.x)/2 - nano_usb_plug_delta/2, 0, nano_plate_size.z ])
            Z0cube(nano_usb_plug_size);
        // ICSP plug
        color("black")
            translate([ (nano_plate_size.x - nano_ICSP_plug_size.x)/2 + nano_usb_plug_delta/2, 0, nano_plate_size.z ])
            Z0cube(nano_ICSP_plug_size);
    }
    if (bbox) {
        color("gold", 0.2) Z0cube(nano_size);
    }
}

magnet_size = [ 23, 11, 3 ];
module magnet(bbox=false) {
    color("silver") Z0cube(magnet_size);
}

circuit_plate_size = [41, 17, 1.6];
circuit_b1_pos = [ -12, -1.25, circuit_plate_size.z/2 ];
circuit_b2_pos = [  12, -1.25, circuit_plate_size.z/2 ];
module circuit() {
    color([0, 0.7, 0])
        Z0cube(circuit_plate_size);
    translate(circuit_b1_pos) button(bbox);
    translate(circuit_b2_pos) button(bbox);
}

between_nano_and_screen = [ (screen_size.x-nano_size.x)/2 + 3, -5, (screen_size.y-nano_size.y)/2 + 1 ];
screen_bbox = computeBbox(screen_size, d=[ 0, -1, 0 ], t=between_nano_and_screen);
nano_bbox = computeBbox(nano_size, d=[ 0, 1, 0 ]);
module nanoAndScreen() {
    moveZTo(d=[ 0,  1, 0 ])
        nano(bbox);
    translate(between_nano_and_screen)
        moveZTo(d=[ 0, -1, 0 ])
        screen(bbox);
}

// components position

bbox=false;

nanoAndScreen();

// bounding box before translation
circuit_bbox_0 = computeBbox(circuit_plate_size);
// align front with screen , stack on top of screen
circuit_translation =
      align(target = circuit_bbox_0, source = screen_bbox, d = [0, -1, 0], margin = 0)
    + stack(target = circuit_bbox_0, source = screen_bbox, d = [0,  0, 1], margin = 1);

moveZTo(o=circuit_translation, d=[0,0,1]) circuit();

moveZTo(o=[-22, 11,  10], d=[ 1,0,0]) batteryAAA();
moveZTo(o=[ 22, 11, -10], d=[-1,0,0]) batteryAAA();

moveZTo(o=[  0, 16,   0], d=[ 0,1,0]) magnet();

// showBBox(unionBbox(screen_bbox, nano_bbox));