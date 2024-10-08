package main

import (
	"fmt"
	"log"
	"math"
	"strconv"
	"strings"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/jmetzg11alpine/sensors_prototype/backend/handlers"
)

var messagePubHandler mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
	fmt.Printf("Received message: %s from topic: %s\n", msg.Payload(), msg.Topic())
}

func connectMQTT() mqtt.Client {
	opts := mqtt.NewClientOptions()
	opts.AddBroker("tcp://localhost:1883") // Replace with your broker's IP
	opts.SetClientID("go_mqtt_client")
	opts.SetDefaultPublishHandler(messagePubHandler)

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	} else {
		fmt.Println("Successfully connected to MQTT broker")
	}

	return client
}

func main() {

	go func() {
		var prevDistance float64
		client := connectMQTT()

		messageHandler := func(client mqtt.Client, msg mqtt.Message) {
			payload := string(msg.Payload())
			parts := strings.Split(payload, " - ")
			if len(parts) > 0 {
				currentDistance, err := strconv.ParseFloat(parts[0], 64)
				if err != nil {
					fmt.Println("Error parsing distance:", err)
					return
				}
				if math.Abs(currentDistance-prevDistance) > 2 {
					fmt.Printf("%s\n", payload)
					prevDistance = currentDistance
				}
			}
		}

		topic := "esp32/output"
		token := client.Subscribe(topic, 1, messageHandler)
		token.Wait()
		fmt.Printf("Subscribed to topic: %s\n", topic)

		// Keep the application running to receive messages
		select {}
	}()

	router := gin.Default()

	router.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"*"},
		AllowMethods:     []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Content-Type", "Accept"},
		ExposeHeaders:    []string{"Content-Length"},
		AllowCredentials: true,
	}))

	router.GET("/", handlers.Home)
	router.GET("/micro/:id", handlers.ReadMicro)

	router.Run("localhost:8080")
}
