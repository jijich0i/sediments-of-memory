# Sediments of Memory

Arduino-controlled interactive installation translating hand-crank motion into sequential LED illumination.

VIS 110A · UC San Diego · 2026 · Exhibited at Mandeville Gallery, UCSD

## What it does

A viewer turns a hand crank, which gradually illuminates 20 zones of LED light across the installation. Each zone is filtered through differently colored materials. As the crank turns more, more zones light up — and as it slows, the light fades back.

The piece explores how memory settles in the body, inspired by Bessel van der Kolk's *The Body Keeps the Score*, and reflects on Alzheimer's disease through physical interaction with light.

## How it works

- Analog sensor reads hand-crank rotation speed
- Sensor value is smoothed and mapped to a zone level (0–20)
- Each of the 20 LED zones illuminates sequentially based on crank input
- Zone brightness transitions gradually (fade in/out) rather than snapping on/off
- Each zone cycles through hue over time while active

## Hardware

- Arduino
- WS2812 LED strip (112 LEDs, 20 zones)
- Analog rotation sensor
- Colored paper/material filters

## Dependencies

FastLED

## Demo

https://youtube.com/shorts/0k6btfgf7tw
