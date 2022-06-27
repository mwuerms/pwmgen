/**
 * Martin Egli
 * 2022-06-27
 */
 
use <parts.scad>
//arduino_nano();
//rotary_encoder();
//rotary_encoder_knob();
//oled_module();
//bread_board(20, 11);
//button();

module electronics1(loc_res = 32) {
    bread_board(29,13);
    color("Green") {
        translate([1*2.54, 1*2.54, -1])
        cylinder(d = 3, h = 3, $fn = loc_res);
        translate([1*2.54, 11*2.54, -1])
        cylinder(d = 3, h = 3, $fn = loc_res);
        translate([27*2.54, 1*2.54, -1])
        cylinder(d = 3, h = 3, $fn = loc_res);
        translate([27*2.54, 11*2.54, -1])
        cylinder(d = 3, h = 3, $fn = loc_res);
    }
    translate([0, 9*2.54, 0])
    rotate([180, 0, 0])
    arduino_nano();
    
    translate([8*2.54, 7*2.54, 1.5])
    rotary_encoder();
    
    translate([18*2.54, 1*2.54, 1.5])
    oled_module();
    
    translate([23*2.54, 5*2.54, 1.5])
    button();
}

module top_case1(show_el = 1, loc_res = 32) {
    if(show_el)
        electronics1();
    color("Green") {
        difference() {
            union() {
                translate([1*2.54, 1*2.54, 1.5])
                cylinder(d = 6, h = 5, $fn = loc_res);
                translate([1*2.54, 11*2.54, 1.5])
                cylinder(d = 6, h = 5, $fn = loc_res);
                translate([27*2.54, 1*2.54, 1.5])
                cylinder(d = 6, h = 5, $fn = loc_res);
                translate([27*2.54, 11*2.54, 1.5])
                cylinder(d = 6, h = 5, $fn = loc_res);
            
                difference() {
                    hull() {
                        translate([1*2.54, 1*2.54, 0])
                        cylinder(d = 14, h = 6.5, $fn = loc_res);
                        translate([1*2.54, 11*2.54, 0])
                        cylinder(d = 14, h = 6.5, $fn = loc_res);
                        translate([27*2.54, 1*2.54, 0])
                        cylinder(d = 14, h = 6.5, $fn = loc_res);
                        translate([27*2.54, 11*2.54, 0])
                        cylinder(d = 14, h = 6.5, $fn = loc_res);
                    }
                    hull() {
                        translate([1*2.54, 1*2.54, -1])
                        cylinder(d = 12, h = 6.5, $fn = loc_res);
                        translate([1*2.54, 11*2.54, -1])
                        cylinder(d = 12, h = 6.5, $fn = loc_res);
                        translate([27*2.54, 1*2.54, -1])
                        cylinder(d = 12, h = 6.5, $fn = loc_res);
                        translate([27*2.54, 11*2.54, -1])
                        cylinder(d = 12, h = 6.5, $fn = loc_res);
                    }
                }
                translate([-4.46, 4*2.54, -2.2])
                cube([1, 4*2.54, 6.5]);
            }
                
            translate([1*2.54, 1*2.54, -1])
            cylinder(d = 3.2, h = 6.5, $fn = loc_res);
            translate([1*2.54, 11*2.54, -1])
            cylinder(d = 3.1, h = 6.5, $fn = loc_res);
            translate([27*2.54, 1*2.54, -1])
            cylinder(d = 3.1, h = 6.5, $fn = loc_res);
            translate([27*2.54, 11*2.54, -1])
            cylinder(d = 3.1, h = 6.5, $fn = loc_res);
            
            translate([25*2.54, 6*2.54, 1.5])
            cylinder(d = 4, h = 6, $fn = loc_res);
            
            translate([5*2.54, 6*2.54, 1.5])
            cylinder(d = 7, h = 6, $fn = loc_res);
            
            translate([18*2.54-16.4, 1*2.54+3.4, 1.5])
            cube([25.3, 17, 6]);
            
            translate([28*2.54, 9*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 7*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 5*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 3*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
        }
    }
}

module bottom_case1(show_el = 1, loc_res = 32) {
    if(show_el)
        electronics1();
    
    color("Green") {
        difference() {
            union() {
                translate([1*2.54, 1*2.54, -6.5])
                cylinder(d = 6, h = 6.5, $fn = loc_res);
                translate([1*2.54, 11*2.54, -6.5])
                cylinder(d = 6, h = 6.5, $fn = loc_res);
                translate([27*2.54, 1*2.54, -6.5])
                cylinder(d = 6, h = 6.5, $fn = loc_res);
                translate([27*2.54, 11*2.54, -6.5])
                cylinder(d = 6, h = 6.5, $fn = loc_res);
                
                difference() {
                    hull() {
                        translate([1*2.54, 1*2.54, -6.5-1])
                        cylinder(d = 14, h = 6.5+1, $fn = loc_res);
                        translate([1*2.54, 11*2.54, -6.5-1])
                        cylinder(d = 14, h = 6.5+1, $fn = loc_res);
                        translate([27*2.54, 1*2.54, -6.5-1])
                        cylinder(d = 14, h = 6.5+1, $fn = loc_res);
                        translate([27*2.54, 11*2.54, -6.5-1])
                        cylinder(d = 14, h = 6.5+1, $fn = loc_res);
                    }
                    hull() {
                        translate([1*2.54, 1*2.54, -6.5])
                        cylinder(d = 12, h = 6.5+1, $fn = loc_res);
                        translate([1*2.54, 11*2.54, -6.5])
                        cylinder(d = 12, h = 6.5+1, $fn = loc_res);
                        translate([27*2.54, 1*2.54, -6.5])
                        cylinder(d = 12, h = 6.5+1, $fn = loc_res);
                        translate([27*2.54, 11*2.54, -6.5])
                        cylinder(d = 12, h = 6.5+1, $fn = loc_res);
                    }
                }
            }
            translate([-2*2.54, 4*2.54, -6.5])
            cube([2*2.45, 4*2.54, 7.5]);
            
            translate([1*2.54, 1*2.54, -8.5]) {
                cylinder(d = 3.1, h = 12, $fn = loc_res);
                cylinder(d2 = 3.1, d1 = 8, h = 3, $fn = loc_res);
            }
            translate([1*2.54, 11*2.54, -8.5]) {
                cylinder(d = 3.1, h = 12, $fn = loc_res);
                cylinder(d2 = 3.1, d1 = 8, h = 3, $fn = loc_res);
            }
            translate([27*2.54, 1*2.54, -8.5]) {
                cylinder(d = 3.1, h = 12, $fn = loc_res);
                cylinder(d2 = 3.1, d1 = 8, h = 3, $fn = loc_res);
            }
            translate([27*2.54, 11*2.54, -8.5]) {
                cylinder(d = 3.1, h = 12, $fn = loc_res);
                cylinder(d2 = 3.1, d1 = 8, h = 3, $fn = loc_res);
            }
            translate([28*2.54, 9*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 7*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 5*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
            translate([28*2.54, 3*2.54-2.54/2, -1.4])
            cube([2*2.54, 1*2.54, 2*2.54]);
        }
    }
}

//top_case1(0);
//bottom_case1(0);
rotary_encoder_knob();
