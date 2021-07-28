const baseURL = "https://api.particle.io/v1/devices/2a002f001047353138383138/";
const accessToken = "putAccessTokenHere";
const numRows = 15;
const numCols = 20;

var currR = 255;
var currG = 255;
var currB = 255;
var lastSentArgs = "";

$(document).ready(function () {
  //create the pixel grid :)
  let grid = "";
  for (let y = 0; y < numRows; y++) {
    grid += `<div class="row" data-pixelY="${y}">`;
    for (let x = 0; x < numCols; x++) {
      grid += `<div class="pixel" id="pixel-${x}-${y}" data-pixelX="${x}" draggable="false"></div>`;
    }
    grid += "</div>";
  }
  $("#grid").html(grid);

  $(".pixel").mousedown(mouseHandler);
  $(".pixel").mouseenter(mouseHandler);

  $("#color").change(function (e) {
    let color = hexToRgb($(this).val());
    currR = color.r;
    currG = color.g;
    currB = color.b;
  });

  $("#image-upload").change(function (e) {
    getFile((dataUrl) => {
      let img = new Image();
      img.src = dataUrl;
      img.onload = function () {
        //draw to canvas at height width
        let canvas = $("#image-canvas");
        let ctx = canvas[0].getContext("2d");

        ctx.canvas.height = numRows;
        ctx.canvas.width = numCols;
        ctx.drawImage(img, 0, 0, numCols, numRows);

        //get individual pixels
        let pixData = ctx.getImageData(0, 0, numCols, numRows);
        let pixels = convertDataToPix(pixData.data);
        let y = 0;
        let x = 0;
        for (let i = 0; i < pixels.length; i++) {
          updatePixel(x, y, pixels[i].r, pixels[i].g, pixels[i].b);
          if (x == pixData.width - 1) {
            y++;
            x = 0;
          } else {
            x++;
          }
        }
      };
    });
  });

  $("#clear").click(function () {
    $.ajax({
      url: baseURL + "clearBoard",
      type: "post",
      dataType: "application/x-www-form-urlencoded",
      data: `access_token=${accessToken}&args=`,
    }).always((data) => {
      console.log(data);
    });
    $(".pixel").css("background-color", "black");
  });
});

function updatePixel(x, y, r, g, b, checkLast) {
  let args = `${String(x).padStart(3, "0")}${String(y).padStart(
    3,
    "0"
  )}${String(r).padStart(3, "0")}${String(g).padStart(3, "0")}${String(
    b
  ).padStart(3, "0")}`;
  if ((checkLast && args != lastSentArgs) || !checkLast) {
    $(`#pixel-${x}-${y}`).css("background-color", `rgb(${r},${g},${b})`);
    $.ajax({
      url: baseURL + "setPixel",
      type: "post",
      dataType: "application/x-www-form-urlencoded",
      data: `access_token=${accessToken}&args=${args}`,
    }).done((data) => {
      console.log(data);
    });
    lastSentArgs = args;
  }
}

function convertDataToPix(data) {
  let finalPix = [];
  let valNum = 0;
  let pix = {};
  for (i = 0; i < data.length; i++) {
    if (valNum == 0) {
      pix.r = data[i];
    } else if (valNum == 1) {
      pix.g = data[i];
    } else if (valNum == 2) {
      pix.b = data[i];
      finalPix.push(pix);
      pix = {};
    }

    if (valNum == 3) {
      valNum = 0;
    } else {
      valNum++;
    }
  }

  return finalPix;
}

function hexToRgb(hex) {
  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result
    ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16),
      }
    : null;
}

function mouseHandler(e) {
  if (e.buttons > 0) {
    let x = $(this).attr("data-pixelX");
    let y = $(this).parent().attr("data-pixelY");
    updatePixel(x, y, currR, currG, currB, true);
  }
}

function getFile(callback) {
  let input = document.getElementById("image-upload");
  if (!input.files[0]) {
    alert("No file selected!");
  } else {
    let file = input.files[0];
    let fr = new FileReader();
    fr.onloadend = function () {
      callback(fr.result);
    };
    let fileUrl = fr.readAsDataURL(file);
  }
}
