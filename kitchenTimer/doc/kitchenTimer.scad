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


// The idea is to externalize all dimensions from rendering modules, to make them available for further computations
//
// Convention for a component name "xyz" :
// - module "xyz" renders component with X & Y centered, bottom at Z=0
// - variable "xyz_size" contains component bounding box with X & Y centered, bottom at Z=0
// - variables "xyz_something_size" may contain sub components bounding box
// - variables "xyz_d" and "xyz_t" are direction and translation vectors to place component
// - variable "xyz_bbox_0" is bounding box at origin
// - variable "xyz_bbox" is bounding box at target position

// batteryAAA

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

// 7 segments screen

screen_size = [ 38, 19, 7 ];

module screen(bbox=false) {
    color("white") Z0cube(screen_size);
    if (bbox) {
        color("gold", 0.5) Z0cube(screen_size);
    }
}

// Button

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

// Circuit with buttons

circuit_plate_size = [41, 17, 1.6];
circuit_b1_t = [ -12, -1.25, circuit_plate_size.z ];
circuit_b2_t = [  12, -1.25, circuit_plate_size.z ];
circuit_size = [ circuit_plate_size.x, circuit_plate_size.y, circuit_plate_size.z + button_size.z ];
circuit_bbox_0 = unionBbox([
    computeBbox(circuit_plate_size),
    computeBbox(button_size, t=circuit_b1_t),
    computeBbox(button_size, t=circuit_b2_t)
]);

module circuit() {
    color([0, 0.7, 0])
        Z0cube(circuit_plate_size);
    translate(circuit_b1_t) button(bbox);
    translate(circuit_b2_t) button(bbox);
}

// Arduino Nano

nano_usb_plug_delta = 2;

nano_plate_size = [ 43, 17.5, 1.5 ];
nano_plate_t = [ nano_usb_plug_delta/2, 0, 0];
nano_plate_bbox = computeBbox(nano_plate_size, t=nano_plate_t);

nano_usb_plug_size = [ 9.5, 7.5, 4 ];
nano_usb_plug_t = [ (nano_usb_plug_size.x-nano_plate_size.x)/2 - nano_usb_plug_delta/2, 0, nano_plate_size.z ];
nano_usb_plug_bbox = computeBbox(nano_usb_plug_size, t=nano_usb_plug_t);

nano_ICSP_plug_size = [ 5, 8, 9 ];
nano_ICSP_plug_t = [ (nano_plate_size.x-nano_ICSP_plug_size.x)/2 + nano_usb_plug_delta/2, 0, nano_plate_size.z ];
nano_ICSP_plug_bbox = computeBbox(nano_ICSP_plug_size, t=nano_ICSP_plug_t);

nano_size = [ nano_plate_size.x+nano_usb_plug_delta, nano_plate_size.y, nano_plate_size.z + max(nano_usb_plug_size.z, nano_ICSP_plug_size.z) ];
// nano_bbox_0 = unionBbox([ nano_plate_bbox, nano_usb_plug_bbox, nano_ICSP_plug_bbox ]);

module nano(bbox=false) {
    union() {
        // plate
        color("blue")
            translate(nano_plate_t)
            Z0cube(nano_plate_size);
        // usb plug
        color("silver")
            // USB plug on left of plate, but outside of nano_usb_plug_delta
            translate(nano_usb_plug_t)
            Z0cube(nano_usb_plug_size);
        // ICSP plug
        color("black")
            translate(nano_ICSP_plug_t)
            Z0cube(nano_ICSP_plug_size);
    }
    if (bbox) {
        color("gold", 0.2) Z0cube(nano_size);
    }
}

// Arduino and screen are soldered together

between_nano_and_screen = [ (screen_size.x-nano_size.x)/2 + 3, -5, (screen_size.y-nano_size.y)/2 + 1 ];
// TODO : = align x + margin -3 & stack y + margin 5
screen_bbox = computeBbox(screen_size, d=[ 0, -1, 0 ], t=between_nano_and_screen);
nano_bbox = computeBbox(nano_size, d=[ 0, 1, 0 ]);

module nanoAndScreen() {
    moveZTo(d=[ 0,  1, 0 ])
        nano(bbox);
    translate(between_nano_and_screen)
        moveZTo(d=[ 0, -1, 0 ])
        screen(bbox);
}

// magnet

magnet_size = [ 23, 11, 3 ];

module magnet(bbox=false) {
    color("silver") Z0cube(magnet_size);
}

// components position

bbox=false;

// align front with screen , stack on top of screen
circuit_t =
      align(target = circuit_bbox_0, source = screen_bbox, d = [ 0, -1, 0 ], margin = 2)
    + stack(target = circuit_bbox_0, source = screen_bbox, d = [ 0,  0, 1 ], margin = 1);

circuit_bbox = computeBbox(circuit_size, t = circuit_t);

battery1_t = [-batteryAAA_size.z/2, 12,  10 ]; battery1_d = [ 1,0,0 ];
battery1_bbox = computeBbox(batteryAAA_size, d = battery1_d, t = battery1_t);

battery2_t = [ batteryAAA_size.z/2, 12, -10 ]; battery2_d = [ -1,0,0 ];
battery2_bbox = computeBbox(batteryAAA_size, d = battery2_d, t = battery2_t);

magnet_t = [ 0, 16, 0 ]; magnet_d = [ 0,1,0 ];
magnet_bbox = computeBbox(magnet_size, d = magnet_d, t = magnet_t);

allBbox = unionBbox([
    screen_bbox,
    nano_bbox,
    circuit_bbox,
    battery1_bbox,
    battery2_bbox,
    magnet_bbox
]);
// showBBox(allBbox);

difference() {
    color("white", 1) createCase(
        allBbox,
        inner_low_margin  = [  2, 2  , 0],
        inner_high_margin = [  2, 2  , 3],
        outer_low_margin  = [  4,-0.1, 2],
        outer_high_margin = [  4, 1  ,-1]
    );
    union() {
        nanoAndScreen();
        moveZTo(o=circuit_t, d=[ 0,0,1 ]) circuit();
        moveZTo(o=battery1_t, d=battery1_d) batteryAAA();
        moveZTo(o=battery2_t, d=battery2_d) batteryAAA();
        moveZTo(o=magnet_t, d=magnet_d) magnet();
    }
};