/**
 * Martin Egli
 * 2022-06-26
 */
// orogin: pin1 = D13
module arduino_nano(loc_res = 32) {
    translate([-4, -2.54/2, 2.5]) {
        color("Red")
        translate([4, 2.54/2, -1-2.5])
        cylinder(d1 = 0, d2 = 1, h= 5, $fn = loc_res);
        
        color("Blue")
        translate([0, 0, 0])
        cube([43.5, 18, 1]);
        
        color("Silver") 
        translate([-1, 5, 1])
        cube([6, 8, 3]);
        
        color("Silver") 
        translate([25, 6, 1])
        cube([4, 6, 2.5]);
        
        color("Black") 
        translate([11, 9, 1])
        rotate([0, 0, -45])
        cube([7, 7, 1]);
        
        color("Black") {
            translate([3, 0, -2.5])
            cube([38, 2.5, 2.5]);
            translate([3, 18-2.5, -2.5])
            cube([38, 2.5, 2.5]);
        }
    }
}

// origin: pin1
module rotary_encoder(loc_res = 32) {
    translate([-((6*2.54-12)/2+12), -(6+2.54), -1]) {
        color("Red")
        translate([(6*2.54-12)/2+12, 6+2.54, -1])
        cylinder(d1 = 0, d2 = 1, h= 5, $fn = loc_res);
        
        color("Silver") {
            translate([0, 0, 0])
            cube([12, 12, 4]);
            translate([6, 6, 0])
            cylinder(d = 6, h = 10, $fn = loc_res);
            difference() {
                hull() {
                    translate([6, 6, 10.5])
                    cylinder(d = 6, h = 12, $fn = loc_res);
                    translate([6, 6, 10.5+12])
                    cylinder(d = 4, h = 1, $fn = loc_res);
                }
                translate([2, 0, 12.5])
                cube([8, 5, 12]);
            }
        }
    }
}

module rotary_encoder_knob(loc_res = 64) {
    color("Green") {
        difference() {
            union() {
                translate([0, 0, 0])
                cylinder(d1 = 13, d2 = 10, h = 14, $fn = loc_res);
                translate([0, 0, 14])
                cylinder(d1 = 10, d2 = 6, h = 1, $fn = loc_res);
                translate([0, 0, 0])
                cylinder(d1 = 16, d2 = 15.5, h = 3, $fn = loc_res);
            }
            //axis
            translate([0, 0, -1])
            cylinder(d2 = 6, d1= 6.4, h = 10+1, $fn = loc_res);
            for(a = [0:1:15]) {
                translate([6*sin(a*360/16), 6*cos(a*360/16), 3])
                cylinder(d = 1.5, h = 12, $fn = loc_res);
            }
        }
    }
}

// origin: pin1
module oled_module(loc_res = 32) {
    translate([-(15/2+2.5/2+3*2.5), -1.25, 2.25]) {
        color("Red")
        translate([15/2+2.5/2+3*2.5, 1.25, -3.25])
        cylinder(d1 = 0, d2 = 1, h= 5, $fn = loc_res);
        
        difference() {
            union() {
                color("DarkGray")
                translate([0, 0, 0])
                cube([25, 27, 1]);
                color("Silver") {
                    translate([2, 2, -0.1])
                    cylinder(d = 4, h= 1.2, $fn = loc_res);
                    translate([25-2, 2, -0.1])
                    cylinder(d = 4, h= 1.2, $fn = loc_res);
                    translate([25-2, 27-2, -0.1])
                    cylinder(d = 4, h= 1.2, $fn = loc_res);
                    translate([2, 27-2, -0.1])
                    cylinder(d = 4, h= 1.2, $fn = loc_res);
                }
            }
            translate([2, 2, -0.3])
            cylinder(d = 3, h= 1.6, $fn = loc_res);
            translate([25-2, 2, -0.2])
            cylinder(d = 3, h= 1.4, $fn = loc_res);
            translate([25-2, 27-2, -0.2])
            cylinder(d = 3, h= 1.4, $fn = loc_res);
            translate([2, 27-2, -0.2])
            cylinder(d = 3, h= 1.4, $fn = loc_res);
        }
        color("Black") 
        translate([0, 5, 1])
        cube([25, 16.5, 1.6]);
        
        color("Black") 
        translate([15/2, 0, -2.5])
        cube([10, 2.5, 2.5]);
        
        color("Orange")
        translate([(25-12)/2, 21.5, 1])
        cube([12, 5, 1]);
    }
        
}
//arduino_nano();
//rotary_encoder();
rotary_encoder_knob();
//oled_module();
