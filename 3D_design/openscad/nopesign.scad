/* 

nope sign SCAD file
(c) 2025, Gary Marsh, github.com/gmarsh
CC BY-SA V4.0 license

This code is a mess, but hopefully decipherable. Feel free to clean it up and throw it back at the github if you hate it.

Workflow for making a new sign:

(1) Make logo_2d() point at a SVG for a new logo.
(2) preview quick_preview_3d(), you'll probably need to mess with the positive/negative offsets in frame_2d() for a good looking outline.
(3) preview pcb_2d() and make sure it looks sane, then export it as a SVG. This is the PCB outline you want.
(4) Pull the SVG into Inkscape, disable fills, save it again as a SVG and import it into Kicad as the board outline layer.
(5) Do the same process for pcb_part_clearance_2d() and logo_2d() and bring those into Kicad on reference layers to guide part placement. 
(6) Do electronics nerd shit
(7) Export STEP files from Kicad for the PCB and parts, convert these to STLs, then update pcb_preview() to point at these files.
(8) Come up with all new cutouts in frame_3d() for buttons and switches and everything else. Confirm no interference by intersection()-ing pcb_preview() and frame_3d()/logo_3d().
(9) Once everything looks good, print frame_3d() and logo_3d() together to make your sign.

*/

// MAIN OUTPUTS

// uncomment for the frame (black plastic) part
color("#3F3F3F") frame_3d();
// uncomment for the logo (clear/opaque plastic) part
color("cyan",0.3) logo_3d();

// KICAD OUTPUTS

// board outline
//pcb_outline_2d();
// logo, for figuring out where to place LEDs etc on the board
//logo_2d();
// part clearance, don't place parts outside this area
//pcb_part_clearance_2d();


// OTHER OUTPUTS

// quick 3D preview for adjusting stuff. Doesn't have PCB or switch cutouts or anything, but renders quick.
//quick_3d_preview();
// PCB preview
//pcb_preview();



// first bit of configuration

// height of plastic above PCB top surface
main_height = 12;
// depth of plastic beside/behind PCB, make sure is >= PCB thickness (typically 1.6mm)
// total sign thicknness is main_height + back_depth
back_depth = 2;

// thickness of clear part. Thinner less light attention, thicker stronger and better diffusion.
logo_thickness = 4.8;
// how much smaller PCB is than the frame
pcb_offset = -1;
// gap between PCB outline and where it goes in
pcb_edge_clearance = 0.2;

// extra PCB clearance around the logo for parts etc
pcb_part_clearance_offset = 2;
pcb_part_clearance_height = 2;


module logo_3d() {
    difference() {
        translate([0,0,main_height]) mirror([0,0,1]) linear_extrude(height=logo_thickness) logo_2d();
        switch_cutouts();
    }
}



module frame_3d() {

    difference() {
    
        // main frame blank w/hangers added
        union() {
            translate([0,0,-back_depth]) linear_extrude(height=main_height + back_depth,convexity=5) difference() { frame_2d(); logo_2d(); }
            mounts();
        }
        
        // PCB cutout
        mirror([0,0,1]) linear_extrude(height=back_depth+1) offset(r=pcb_offset+pcb_edge_clearance) frame_2d();
        // PCB part clearance, 2mm above PCB, which clears most parts
        translate([0,0,-1]) linear_extrude(height=pcb_part_clearance_height+1) offset(r=pcb_part_clearance_offset) logo_2d();
        // PCB mounting holes
        pcb_holes();
        // USB cutout
        usb_cutout();
        // switch cutouts
        switch_cutouts();
        
    }
}

// string hangey dangleys
module mounts() {
    translate([-46.8,51.8,0]) rotate([0,0,10.5]) mount();
    translate([64.3,47.7,0])  rotate([0,0,-15.75]) mount();
}

module mount() {
    od = 7; id = 3;
    dx = 5.5; dy = 3;
    
    difference() {
        translate([0,0,-back_depth]) linear_extrude(height=main_height+back_depth,convexity=5) difference() {
            hull() {
                circle(d=od,$fn=32);
                translate([-dx,-dy]) circle(d=1,$fn=32);
                translate([dx,-dy]) circle(d=1,$fn=32);
            }
            circle(d=id,$fn=32);
        }
        
        translate([0,-id/2,main_height])  rotate([-45,0,0]) linear_extrude(height=10) square([3*od,3*od],center=true);
        translate([0,-id/2,-back_depth])  mirror([0,0,1]) rotate([-45,0,0]) linear_extrude(height=10) square([3*od,3*od],center=true);
    }
}


// difference between kicad coordinates and openscad coordinates, used in a bunch of functions below
part_dx = -150;
part_dy = 85;

module pcb_holes() {
    nudge = 0.1;
    // hole depth and diameter
    hole_depth = main_height - 2;
    hole_dia = 3.3;
    
    translate([0,0,-nudge]) linear_extrude(height=hole_depth+nudge) union() { 
        translate([66.5+part_dx,-60.5+part_dy]) circle(d=hole_dia,$fn=32);
        translate([56+part_dx,-100+part_dy]) circle(d=hole_dia,$fn=32);
        translate([108+part_dx,-49+part_dy]) circle(d=hole_dia,$fn=32);
        translate([157+part_dx,-87+part_dy]) circle(d=hole_dia,$fn=32);
        translate([199.5+part_dx,-122+part_dy]) circle(d=hole_dia,$fn=32);
        translate([239.5+part_dx,-104+part_dy]) circle(d=hole_dia,$fn=32);
        translate([244+part_dx,-70+part_dy]) circle(d=hole_dia,$fn=32);
    }
}

module usb_cutout() {
    // USB cutout
    usb_x = 104; usb_y = -131;
    usb_angle = -10.5;
    translate([0,0,-0.5]) linear_extrude(height=4) translate([usb_x+part_dx,usb_y+part_dy]) rotate(usb_angle) square([10,10],center=true);
}

// switch cutouts

module switch_cutouts() {
    switch_x = [123,139,155];
    switch_y = [-132.5,-133.5,-134];
    switch_angle = [174,177,180];
    
    for (i=[0:2]) {
        translate([switch_x[i]+part_dx,switch_y[i]+part_dy,0])
        rotate([0,0,switch_angle[i]]) switch_cutout();
    }
}

module switch_cutout() {
    translate([2.25,0,0]) union() {
        translate([0,0,-0.5]) linear_extrude(height=7.5) square([8,6.5],center=true);
        translate([0,0,3.5]) rotate([-90,0,0]) cylinder(d=4.5,$fn=32,h=10);
    }    
}




// quick 3D preview for checking stuff without rendering whole sign

module quick_3d_preview() {

    // mounts / string hangeys
    //color("#3F3F3F") mounts();
    // frame
    color("#3F3F3F") translate([0,0,-back_depth]) linear_extrude(height=main_height+back_depth) difference() { frame_2d(); logo_2d(); };
    // text
    color("#FFBFBF") translate([0,0,main_height]) mirror([0,0,1]) linear_extrude(height=logo_thickness) logo_2d();
}


// PCB preview for test fitment using STLs generated from Kicad

module pcb_preview() {
    union() {
        color("green") translate([-150,85,-1.6]) import("nope_pcb.stl",convexity=5);
        color("red") translate([-150,85,-1.6]) import("nope_pcb_partsonly.stl",convexity=5);
    }
}


module pcb_outline_2d() {
    offset(r=pcb_offset) frame_2d();
}

module pcb_part_clearance_2d() {
    offset(r=pcb_part_clearance_offset) logo_2d();
}

// frame: offset from logo

module frame_2d() {
    offset(r=-1,$fn=60) offset(r=5.5,$fn=60) logo_2d();
}

module logo_2d() {
    translate([-115.0,-150]) import("logo.svg");
}    
