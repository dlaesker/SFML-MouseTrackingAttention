/*
 AUTHOR: Denis Laesker
 DESCRIPTION: This file implements a mouse-tracking paradigm. Upon starting the application, the user is tasked with following randomly appearing circles with the mouse.
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#define FRAME_RATE  	60					
#define WIN_BG_COLOR    sf::Color::White	// Window background color
#define CRADIUS     	20					// Circle radius
#define NTARGETS		10					// Sets the number of targets

/* NOTES:
 Save the initial (Pi) and final (Pf) positions of the mouse, where the last position corresponds to the moment the mouse touches the circle. This will later be used to compute the dist(Pi, Pf).
 
 As a game, randomly generated positions for the circle respawn is ok. However, during an experiment, it would be imperative to come up with a list of predefined positions.
 */

int main(void){
    srand(time(NULL)); // Seed the pseudorandom generator
    
	/* Time vector to keep track of the time taken from the initial mouse position to the position of the target circle */
    std::vector<sf::Time> 		ttracker;
	
	/* Vector to track trajectory mouse -> target */
	std::vector<sf::Vector2i>	mpositions;
    
	// Retrieve dimensions of the screen
    sf::VideoMode desktop = sf::VideoMode().getDesktopMode();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
	
    sf::RenderWindow window(
		desktop, "AttentionTracker", sf::Style::Default, settings
	);
	window.setFramerateLimit(FRAME_RATE);
        
    sf::Vector2u winSize = window.getSize();
    
	/* Painting Area */
    sf::Vector2u circlePaintingArea(
		winSize.x - 2*CRADIUS,
		winSize.y - 2*CRADIUS
	);
    
    sf::CircleShape circle;
    circle.setRadius(CRADIUS);
    circle.setPointCount(1000);
    circle.setFillColor(sf::Color::Red);
	circle.setOrigin(0, 0);
    
	// Half the painting area
    int cx = (circlePaintingArea.x / 2);
    int cy = (circlePaintingArea.y / 2);
	circle.setPosition(cx, cy);
	
	sf::Mouse::setPosition(sf::Vector2i(cx,cy), window);
	mpositions.push_back(sf::Vector2i(cx,cy));
    
    int PAINT 		= 1;
	int CTARGET		= 0;
    
	sf::Event ev;		// To be used when polling events
    sf::Clock clock; 	// To time mouse to circle trajectory
    
	while(window.isOpen()){ // Display the window
		if(CTARGET == NTARGETS) window.close();
		while(window.pollEvent(ev)){ // Poll for events
			switch(ev.type){
                case sf::Event::Closed:{
                    window.close();
                    break;
                }
				case sf::Event::MouseMoved:{
					sf::Vector2f cPos = circle.getPosition();
                    
/* I use the Euclidan distance d = sqrt((Mx - Cx)^2 + (My - Cy)^2), where Mx,y and Cx,y are the locations of the mouse and circle in the Cartesian plane, respectively. To determine whether the mouse is inside the circle, we may compare d < r, or alternatively d^2 < r^2 to avoid having to compute the square root. However, d does not account for the radius. This is corrected by subtracting the radius from the current mouse position. Thus, sqrt(((Mx - r) - Cx)^2 + ((My - r) - Cy)^2). */
					
					int dx = pow(((ev.mouseMove.x - CRADIUS) - cPos.x),2);
                    int dy = pow(((ev.mouseMove.y - CRADIUS) - cPos.y),2);
                    
                    int d = dx + dy;
                    
                    // Is the mouse inside the circle?
                    if(d < CRADIUS*CRADIUS){
						ttracker.push_back(clock.getElapsedTime());
                        
						// Generate next position
                        cx = rand() % circlePaintingArea.x;
                        cy = rand() % circlePaintingArea.y;
                        
						// Update circle's position
                        circle.setPosition(cx, cy);
						
						// Set the paint flag to true
                        PAINT = 1;
						CTARGET += 1;
						
						mpositions.push_back(sf::Vector2i(
							ev.mouseMove.x,ev.mouseMove.y)
						);
						
                    }
					
					break;
                }
            }
        }
        
        /* DRAWING */
        if(PAINT){
            window.clear(WIN_BG_COLOR);
            window.draw(circle);
            PAINT = 0;
            clock.restart();
			window.display();
        }
        
    }
	
//	Code below prints out time and position information
	
//    int i;
//    for(i = 0; i < ttracker.size(); ++i)
//		printf("%f\n", ttracker[i].asSeconds());
//
//	for(i = 0; i < mpositions.size(); ++i)
//		printf("%d\t%d\n", mpositions[i].x, mpositions[i].y);
    
	return 0;
}
