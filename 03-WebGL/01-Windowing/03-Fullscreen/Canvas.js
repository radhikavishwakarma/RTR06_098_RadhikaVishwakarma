var canvas = null;
var context = null;
var bFullscreen = false;

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

    // Get 2D context from canvas
    context = canvas.getContext("2d");
    if (context == null)
    {
        console.log("context element cant be optained");
    }
    else
    {
        console.log("context element succesfully obtained");
    }

    // Tell context to make canvas background black
    context.fillStyle = "black"; // We can use "#000000"
    context.fillRect(0, 0, canvas.width, canvas.height);

    // call drawText
    drawText("Hello World!!");

    // Register our callback fuction as event listeners
    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);

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
                    drawText("Hello World!!");
                    bFullscreen = true;  
            }
            else
            {
                toggleFullscreen();
                drawText("Hello World!!");
                bFullscreen = false;
            }
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

function drawText(text)
{
    // code
    // show the text centrally, set the properties accordingly]
    context.textAlign = "center"; // this sets text center horizontally
    context.textBaseLine = "middle"; // this sets text center vertically
    
    // set the text font
    context.font = "48px sans-serif";
    // set color of the text
    context.fillStyle = "#55ff00ff";
    // set the text
    context.fillText(text, canvas.width/2, canvas.height/2);

}
}


