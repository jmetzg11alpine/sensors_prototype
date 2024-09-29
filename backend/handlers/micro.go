package handlers

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func Home(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"message": "backend is running",
	})
}

func ReadMicro(c *gin.Context) {
	sensorID := c.Param("id")

	c.JSON(http.StatusOK, gin.H{
		"sensor_id": sensorID,
		"status":    "success",
		"data":      "some data",
	})
}
