const express = require("express");
const app = express();

app.get("/", (req, res) => {
  console.log("gamestats microservice root ");  
});

app.get("/api/v1/gamestats", async (req, res) => {
  console.log("gamestats microservice api");
});

module.exports = app;
