package handlers

import (
	"math/rand"
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

	animals := []string{"dog", "cat", "bird", "lizzard", "frog"}

	randomAnimal := animals[rand.Intn(len(animals))]

	c.JSON(http.StatusOK, gin.H{
		"sensor_id": sensorID,
		"status":    "success",
		"data":      randomAnimal,
	})
}
