# Pixel Steamer

This is an experiment for how quickly I can pump function calls into the Particle API

In this implementation, I simply accept a web function call and set a specified pixel from that API call to the color specified

## Web UI

To try this out and control it, I created a webUI that allows you to draw on a board to set various pixel values

To setup the web UI, you'll have to include your particle API keys in the drawingController.js file

The app creates form request ws requests. The API is specified by Particle and allows cross-origin requests, you can simply run this web UI in a local browser, or host it on your local network.

## Creating your own streamer

Send requests to the setPixel function endpoint.

Argument format is a 14 character string for efficiency and due to limitations of Particle function calls
`[pixelX][pixelY][r][g][b]` each parameter should be exactly 3 digits (r,g,b values between 0-255)

To clear the board, send function clearBoard.
