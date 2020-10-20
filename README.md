# plant-tracker
Aggregate, forward iot data

- Run "docker-compose up --build --detach"

## How to use
`POST http://ip/api/v1/push`
Payload: `{"name":"lemon_tree","family":"soil_moisture","value":2.3}`
