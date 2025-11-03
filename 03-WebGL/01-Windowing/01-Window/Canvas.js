function main()
{
    // Get canvas
    var canvas=document.getElementById("AMC")
    if (canvas == null)
    {
        console.log("canvas element cant be optained");
    }
    else{
        console.log("canvas element succesfully obtained");

    }

    // Get 2D context 
    var context=canvas.getContext("2d");
    if (context == null)
    {
        console.log("context element cant be optained");
    }
    else{
        console.log("context element succesfully obtained");

    }

    // Tell context to make canvas background black
    context.fillStyle="black"; // We can use "#000000"
    context.fillRect(0, 0, canvas.width, canvas.height);

    // To show the Text centrally set accordingly
    context.textAlign = "center"; // this sets text center horizontally
    context.textBaseLine = "middle"; // this sets text center vertically

    // Set the text font
    context.font = "48px sans-serif";
    // Set color of the text
    context.fillStyle = "#00ff00" 
    // set the text
    var str = "Hello World!!!";
    context.fillText(str, canvas.width/2, canvas.height/2);


}
