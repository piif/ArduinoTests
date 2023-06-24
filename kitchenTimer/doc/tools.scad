// rotate and translate object :
// - change its Z vector to d. d must be one of X, -X, Y or -Y (or Z keep current direction)
// - translate it of "o" 
module moveZTo(o=[0,0,0], d=[0,0,1]) {
    assert(d==[1,0,0] || d==[-1,0,0] || d==[0,1,0] || d==[0,-1,0] || d==[0,0,1], "d must be one of X, -X, Y, -Y or Z");
    rx = (d.y==1) ? -90 : ( (d.y==-1) ?  90 : 0 );
    ry = (d.x==1) ?  90 : ( (d.x==-1) ? -90 : 0 );
    translate(o) rotate([rx, ry, 0]) children();
}

module moveXTo(o=[0,0,0], d=[0,0,1]) {
    echo("d = " , d[0]);
    ry = (d.z==1) ? -90 : ((d.z==-1) ?  90 : 0);
    rz = (d.y==1) ?  90 : ((d.y==-1) ? -90 : 0);
    translate(o) rotate([0, ry, rz, 0]) children();
}

// cube with X & Y centered, bottom at Z=0
module Z0cube(size) {
    translate([0, 0, size.z/2]) cube(size, center=true);
}

// return a list of 2 points, associated to min and max bounding box points, after given direction and translation
// direction is a unary vector as used in moveZTo
// initial bounding box is considered with X & Y centered, bottom at Z=0
function computeBbox(source_bbox, d=[ 0, 0, 0 ], t=[ 0, 0, 0 ]) =
    let(
        lower_bbox = [
            (d.x== 1) ? 0 : (
            (d.x==-1) ? -source_bbox.z : (
            -source_bbox.x/2
            ))
        ,
            (d.y== 1) ? 0 : (
            (d.y==-1) ? -source_bbox.z : (
            -source_bbox.y/2
            ))
        ,
            (d.x!=0) ? -source_bbox.x/2 : (
            (d.y!=0) ? -source_bbox.y/2 : (
            0
            ))
        ],
        upper_bbox = [
            (d.x== 1) ? source_bbox.z : (
            (d.x==-1) ? 0 : (
            source_bbox.x/2
            ))
        ,
            (d.y== 1) ? source_bbox.z : (
            (d.y==-1) ? 0 : (
            source_bbox.y/2
            ))
        ,
            (d.x!=0) ? source_bbox.x/2 : (
            (d.y!=0) ? source_bbox.y/2 : (
            source_bbox.z
            ))
        ]
    )
    [ lower_bbox + t , upper_bbox + t ];

function unionBbox(bbox1, bbox2) =
    [
        [ min(bbox1[0].x, bbox2[0].x), min(bbox1[0].y, bbox2[0].y), min(bbox1[0].z, bbox2[0].z) ],
        [ max(bbox1[1].x, bbox2[1].x), max(bbox1[1].y, bbox2[1].y), max(bbox1[1].z, bbox2[1].z) ]
    ];

module showBBox(points) {
    color("gold", 0.5) translate(points[0]) cube(points[1]-points[0]);
}

function stack(target, source, d = [ 0, 0, 1 ], margin = 0) =
    [
        d.x == 1 ? source[1].x + margin : (d.x == -1 ? source[0].x - (target[1].x - target[0].x) - margin : 0)
    ,
        d.y == 1 ? source[1].y + margin : (d.y == -1 ? source[0].y - (target[1].y - target[0].y) - margin : 0)
    ,
        d.z == 1 ? source[1].z + margin : (d.z == -1 ? source[0].z - (target[1].z - target[0].z) - margin : 0)
    ];

function align(target, source, d = [ 0, 0, 1 ], margin = 0) =
    [
        d.x == 1 ? (source[1].x - target[1].x - margin) : (d.x == -1 ? (source[0].x - target[0].x + margin) : 0)
    ,
        d.y == 1 ? (source[1].y - target[1].y - margin) : (d.y == -1 ? (source[0].y - target[0].y + margin) : 0)
    ,
        d.z == 1 ? (source[1].z - target[1].z - margin) : (d.z == -1 ? (source[0].z - target[0].z + margin) : 0)
    ];