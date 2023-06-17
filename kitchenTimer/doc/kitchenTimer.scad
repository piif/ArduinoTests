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

module moveZTo(o=[0,0,0], d=[0,0,1]) {
    echo("d = " , d[0]);
    rx = (d.y==1) ? -90 : ((d.y==-1) ?  90 : 0);
    ry = (d.x==1) ?  90 : ((d.x==-1) ? -90 : 0);
    translate(o) rotate([rx, ry, 0]) children();
}
module moveXTo(o=[0,0,0], d=[0,0,1]) {
    echo("d = " , d[0]);
    ry = (d.z==1) ? -90 : ((d.z==-1) ?  90 : 0);
    rz = (d.y==1) ?  90 : ((d.y==-1) ? -90 : 0);
    translate(o) rotate([0, ry, rz, 0]) children();
}

module batteryAAA() {
    color("brown") union() {
        cylinder(d=4, h=44, center=false);
        cylinder(d=10.3, h=42.5, center=false);
    }
}
module screen() {
    color("white") cube([38, 7, 19], center=true);
}

module button() {
    // 6, 3.3, 4, 7
    color("black")
        union() {
            translate([0, 0, 4/2]) cube([6, 6, 4], center=true);
            translate([0, 0, 7/2]) cylinder(d=3.3, h=7, center=true);
        }
}

module nano() {
    union() {
        // plate
        color("blue") translate([-43/2, -17.5/2, 0]) cube([43, 17.5, 1.5]);
        // usb plug
        color("silver") translate([-43/2-1.5, -7.5/2, 1.5]) cube([9.5, 7.5, 4]);
        // ICSP plug
        color("black") translate([43/2-5, -8/2, 1.5]) cube([5, 8, 9]);
    }
}

module magnet() {
    color("silver") translate([0, 1.5, 0]) cube([23, 3, 11], center=true);
}

module circuit() {
    color([0, 0.7, 0])
        translate([0, 0, -1.6/2]) cube([41, 17, 1.6], center=true);
    translate([-12, -1.25, 0]) button();
    translate([ 12, -1.25, 0]) button();
}

module nanoAndScreen() {
    moveZTo(d=[0,1,0]) nano();
    translate([0, -7/2 -5, 0]) screen();
}

moveZTo(o=[-22, 11,  10], d=[ 1,0,0]) batteryAAA();
moveZTo(o=[ 22, 11, -10], d=[-1,0,0]) batteryAAA();
nanoAndScreen();
translate([0, 17/2-12, 12]) circuit();
translate([0, 16, 0]) magnet();
