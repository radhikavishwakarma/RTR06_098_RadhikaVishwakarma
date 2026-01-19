var canvas = null;
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;
var request_animation_frame = window.requestAnimationFrame ||
                                window.mozRequestAnimationFrame ||
                                window.oRequestAnimationFrame ||
                                window.webKitRequestAnimationFrame ||
                                window.msRequestAnimationFrame;

function main()
{
    // Get canvas
    canvas = document.getElementById("AMC")
    if (canvas == null)
    {
        console.log("canvas element cant be optained");
    }
    else{
        console.log("canvas element succesfully obtained");

    }

    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;
    
    // Register our callback fuction as event listeners
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);

    // Initialize
    initialize();

    // Resize
    resize();

    // Display
    display();

    function keyDown(event)
    {
        // code
        switch(event.keyCode)
        {
            case 70:
            case 102:
                if (bFullscreen == false)
                {
                    toggleFullscreen();
                    bFullscreen = true;  
                }
                else
                {
                    toggleFullscreen();
                    bFullscreen = false;
                }
                break; 
            case 69:
                uninitialize();
                window.close();
                break;
            default:
                break;     
        }
    }

    function mouseDown()
    {
        // code
    }

    function toggleFullscreen()
    {
        // code
        var fullscreen_element = 
        document.fullscreenElement ||
        document.mozFullScreenElement ||
        document.webkitFullscreenElement ||
        document.msFullscreenElement ||
        null;

        if (fullscreen_element == null)
        {
            if (canvas.requestFullscreen)
            {
                canvas.requestFullscreen();

            }
            else if (canvas.mozRequestFullScreen)
            {
                canvas.mozRequestFullScreen();

            }    
            else if (canvas.webkitReuestFullscreen)
            {
                canvas.webkitReuestFullscreen();

            }    
            else if (canvas.msReuestFullscreen)
            {
                canvas.msReuestFullscreen();

            }    

        }
        else
        {
            if(document.exitFullscreen)
            {
                document.exitFullscreen();
            }
            else if(document.mozExitFullScreen)
            {
                document.mozExitFullScreen();
            }
            else if(document.webkitExitFullScreen)
            {
                document.webkitExitFullScreen();
            }
            else if(document.msExitFullScreen)
            {
                document.msExitFullScreen();
            }
        }     
    }

    // Stub Function
    function initialize()
    {
        // Code
        // Get 2D context from canvas
        gl = canvas.getContext("webgl2");
        if (gl == null)
        {
            console.log("Webgl2 Context element cant be optained");
        }
        else
        {
            console.log("Webgl2 Context element succesfully obtained");
        }

        // Set viewport width and viewport height
        gl.viewportWidth = canvas.width;
        gl.viewportHeight = canvas.height;

        // Clear color
        gl.clearColor(0.0, 0.0, 1.0, 1.0);
        
    }

    function resize()
    {
        // Code
        if(bFullscreen == true)
        {
            canvas.width = window.innerWidth;
            canvas.height = window.innerHeight;
        }
        else
        {
            canvas.width = canvas_original_width;
            canvas.height = canvas_original_height;
        }

        gl.viewport(0, 0, canvas.width, canvas.height);
    }

    function display()
    {
        // Code
        gl.clear(gl.COLOR_BUFFER_BIT)
        
        // Update call
        update();

        // Double buffering
        request_animation_frame(display, canvas);
    }

    function update()
    {
        // Code
    }

    function uninitialize()
    {
        // Code
    }



}


