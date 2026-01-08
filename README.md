# A Fully Interactive 2D OpenGL City Environment

A dynamic 2D Computer Graphics project built using **C++** and **OpenGL (GLUT)**.  
This project simulates an animated urban environment featuring day/night modes, weather effects, moving vehicles, birds, airplane, windmill, shading, and human interactions.

---

## **Features**

- Multi-layer animated city environment
- Day and Night scene transitions
- Rain, snow, and thunder effects
- Moving cars, birds, boat, and airplane
- Rotating windmill (DDA + Midpoint)
- Human with umbrella toggle under rain
- River water animation
- Parallax mountains
- Street lamps with light cones
- Cloud movement
- Sun & Moon switching
- Window lights during night
- Algorithm-based rendering
- Keyboard interaction for events

---

## **Graphics Algorithms Used**

This project implements classical computer graphics algorithms:

### **1. DDA Line Drawing**
Used for:
- Windmill blades
- Road markings
- Lamp poles
- Building edges

### **2. Midpoint Circle Algorithm**
Used for:
- Windmill hub
- Human head
- Street lamp heads
- Sun / Moon

### **3. Bézier Curves**
Used for:
- Clouds
- Boat waves
- Umbrella arcs

### **4. Conditional Shading**
Day/Night color shading applied to:
- Sky
- Mountains
- Buildings
- Water
- Lamp glow
- Windows
- Sun/Moon

---

## **Scene Overview**

#### **Day Mode**
- Bright sky
- Clouds drifting
- Birds flying
- Cars moving
- Human visible
- Windmill rotating
- Airplane passes over mountains

#### **Night Mode**
- Dark sky shading
- Street lamps glow
- Building windows light up
- Moon replaces sun
- Weather effects more visible

---

## **Controls (Keyboard)**

| Key | Action |
|-----|--------|
| D / d | Switch to Day mode |
| N / n | Switch to Night mode |
| R | Start rain |
| S | Stop rain |
| W / w | Toggle snowfall |
| T | Thunder effect |
| U / u | Toggle umbrella |
| P | Toggle airplane |
| Space | Pause/Resume animation |
| 1-6 | Vehicle movement controls |
| ESC | Exit program |

---

## **Technologies Used**

- C / C++
- OpenGL (GLUT)
- Rasterization pipeline
- 2D animation & transformations
- Event-driven input handling

---

## **Project Objectives**

- Demonstrate practical computer graphics algorithms
- Create interactive and dynamic 2D animation
- Implement environmental simulation using rendering logic
- Provide real-time scene transitions and interactions

---

## **Screenshots**
_(Add screenshots or GIF here if required)_

---

## **How to Run**
1. Install **OpenGL + GLUT**
2. Compile using:
or build via IDE (CodeBlocks / Visual Studio)
3. Run the executable

---



1. Install **OpenGL + GLUT**
2. Compile using:
