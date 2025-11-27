var ws = new WebSocket("ws://localhost:9002");

const statusEl = document.getElementById("status");
statusEl.textContent = "Connecting...";

ws.onopen = function () {
    console.log("Connected to C++ WebSocket++ server");
    statusEl.textContent = "Connected!";
};

const canvas = document.getElementById("noiseCanvas");
const ctx = canvas.getContext("2d");

const perlinCanvas = document.getElementById("perlinCanvas");
const perlinCtx = perlinCanvas.getContext("2d");

const cloudCanvas = document.getElementById("cloudCanvas");
const cloudCtx = cloudCanvas.getContext("2d");

const waterCanvas = document.getElementById("waterCanvas");
const waterCtx = waterCanvas.getContext("2d");

ws.onmessage = (event) => {
  try {
    const msg = JSON.parse(event.data);

    // Expecting { type: "...", payload: { width, height, data: [...] } }
    if (!msg.type || !msg.payload) {
      console.log("Non-image message:", msg);
      return;
    }

    const payload = msg.payload;

    if (!(payload.width && payload.height && payload.data)) {
      console.log("Invalid image payload:", payload);
      return;
    }

    let imgData;
    let ctxTarget;

    if (msg.type === "white") {
      imgData = ctx.createImageData(payload.width, payload.height);
      ctxTarget = ctx;
    } else if (msg.type === "perlin") {
      imgData = perlinCtx.createImageData(payload.width, payload.height);
      ctxTarget = perlinCtx;
    } else if (msg.type === "clouds") {
      imgData = cloudCtx.createImageData(payload.width, payload.height);
      ctxTarget = cloudCtx;
    } else if (msg.type === "water") {
      imgData = waterCtx.createImageData(payload.width, payload.height);
      ctxTarget = waterCtx;
    } else {
      console.log("Unknown type:", msg.type);
      return;
    }

    for (let i = 0; i < payload.data.length; i++) {
      const v = payload.data[i];
      const j = i * 4;

      if (msg.type === "white") {
        const c = v * 255;
        imgData.data[j + 0] = c;
        imgData.data[j + 1] = c;
        imgData.data[j + 2] = c;
        imgData.data[j + 3] = 255;
      } else if (msg.type === "perlin") {
        let min = 999;
        let max = -999;

        for (let i = 0; i < payload.data.length; i++) {
          const v = payload.data[i];
          if (v < min) min = v;
          if (v > max) max = v;
        }

        console.log("PERLIN RANGE -> min:", min, "max:", max);

        // Terrain-style coloring
        let r, g, b;

        if (v < 0.35) {
          r = 0;
          g = 60;
          b = 160 + (v * 60);
        }
        else if (v < 0.6) {
          r = 50 + (v * 60);
          g = 160 + (v * 40);
          b = 60;
        }
        else if (v < 0.8) {
          r = 120 + (v * 80);
          g = 120 + (v * 80);
          b = 120 + (v * 80);
        }
        else {
          r = 220 + (v * 35);
          g = 220 + (v * 35);
          b = 220 + (v * 35);
        }

        imgData.data[j + 0] = r;
        imgData.data[j + 1] = g;
        imgData.data[j + 2] = b;
        imgData.data[j + 3] = 255;
      } else if (msg.type === "clouds") {
        let density = Math.min(Math.max((v - 0.35) * 1.8, 0), 1);

        // Sky color
        const skyR = 120;
        const skyG = 180;
        const skyB = 255;

        // Cloud color
        const cloudR = 255;
        const cloudG = 255;
        const cloudB = 255;

        // Blend sky -> cloud smoothly using density
        const r = skyR + density * (cloudR - skyR);
        const g = skyG + density * (cloudG - skyG);
        const b = skyB + density * (cloudB - skyB);

        imgData.data[j + 0] = r;
        imgData.data[j + 1] = g;
        imgData.data[j + 2] = b;
        imgData.data[j + 3] = 255;
      } else if (msg.type === "water") {
        const x = i % payload.width;
        const y = Math.floor(i / payload.width);
        const depth = y / payload.height;

        // Normalize wave to [-1,1] then soften it
        const wave = (v - 0.5) * 2.0;
        const smoothWave = Math.sin(wave * Math.PI * 0.5);

        // Deep -> Shallow gradient
        const deep = { r: 8,  g: 30,  b: 70 };
        const mid  = { r: 15, g: 80,  b: 150 };
        const top  = { r: 60, g: 140, b: 200 };

        // Vertical depth blend
        let r = deep.r * depth + top.r * (1 - depth);
        let g = deep.g * depth + top.g * (1 - depth);
        let b = deep.b * depth + top.b * (1 - depth);

        // Wave lighting
        let light = 0.85 + smoothWave * 0.25;
        r *= light;
        g *= light;
        b *= light;

        if (v > 0.92) {
          r += 20;
          g += 25;
          b += 30;
        }

        // Clamp
        r = Math.min(255, Math.max(0, r));
        g = Math.min(255, Math.max(0, g));
        b = Math.min(255, Math.max(0, b));

        imgData.data[j + 0] = r;
        imgData.data[j + 1] = g;
        imgData.data[j + 2] = b;
        imgData.data[j + 3] = 255;
      }
    }

    ctxTarget.putImageData(imgData, 0, 0);
    console.log("Rendered:", msg.type);

  } catch (e) {
    console.error("Invalid message:", e, "Raw:", event.data);
  }
};

ws.onerror = function (err) {
    console.error("WebSocket error: ", err);
};

document.addEventListener("DOMContentLoaded", function () {
    var sendBtn = document.getElementById("sendBtn");
    if (sendBtn) {
        sendBtn.addEventListener("click", function () {
            if (ws.readyState === WebSocket.OPEN) {
                ws.send("whiteNoise");
            }
            else {
                console.warn("WebSocket not open yet");
            }
        });
    }
});
