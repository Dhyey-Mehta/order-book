
# OrderBook

## Overview

OrderBook is a high-performance limit order book system designed to manage and match buy/sell orders in financial markets. The system supports real-time data streaming and visualization.

## Features

- **High Throughput Matching Engine:** Developed using C++ to optimize matching engine algorithms for minimal latency and high throughput.
- **Real-time Data Streaming:** Integrated with Apache Kafka to handle real-time data streaming, ensuring robust scalability and fault tolerance.
- **Market Simulation:** Integrated Bristol Stock Exchange to simulate orders for an arbitrary instrument.
- **Visualization:** Provides real-time order book visualization with Next.js and React.js, enabling traders to monitor market dynamics effectively.

## Tech Stack

- **Visualization:** Next.js, React.js, Socket.io
- **Backend:** C++
- **Data Streaming:** Apache Kafka


## Installation

### Prerequisites

- [Next.js](https://nextjs.org/)
- [Python 3.x](https://www.python.org/)
- [Apache Kafka](https://kafka.apache.org/)
- [Modern C++ Kafka](https://github.com/morganstanley/modern-cpp-kafka)

### Steps


1. **Clone the repository:**

   ```bash
   git clone https://github.com/Dhyey-Mehta/OrderBook.git
   cd OrderBook
   ```

2. **Setup Apache Kafka**

   Follow the [official Kafka documentation](https://kafka.apache.org/quickstart) to set up and start Kafka or setup a cluster on [Confluent Cloud](https://confluent.cloud/). Ensure that you create two topics called "trades" and "matches" which will be used for all three components to communicate.

3. **Create src/kafka_config.txt**
   
   Store the connection info for your Kafka instance in this file. It should look something like this:
   ```
   bootstrap.servers=BOOTSTRAP-SERVERS-HERE
   sasl.username=API-USERNAME
   sasl.password=API-PASSWORD
   ```

4. **Compile and run the matching engine:**

   ```bash
   cd src/order_book
   make && ./trading_app
   ```

5. **Install dependencies for the visualization:**

   In a new terminal:
   ```bash
   cd src/visualization/order-book
   npm install
   ```

6. **Run the frontend:**

   ```bash
   cd src/visualization/order-book
   npm run dev
   ```

7. **Install dependencies for the trading client:**
   ```bash
   cd src/trading_client
   pip install -r requirements.txt 
   ```

8. **Run the trading client:**
   ```bash
   python ./client.py
   ```
9. **Access the application:**

   Visit `http://localhost:3000` in your web browser. You should be able to see trades being executed on the order book.

## Project Structure

```plaintext
OrderBook/
│
├── src/                       # Source code directory
│   ├── order_book/            # Order book implementation
│   |── trading_client/        # Trading client implementation
│   |   └── client.py
│   |── visualization/         # Frontend and visualization
│       └── order-book/
│
└── .gitignore                 # Git ignore file
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

