"use client"

import React, { useState, useEffect } from 'react';
import { Bar } from 'react-chartjs-2';
import { Chart, registerables } from 'chart.js';

Chart.register(...registerables);

const LandingPage = () => {
  // Initial example data
  const [orderBook, setOrderBook] = useState({
    labels: ['100', '101', '102', '103', '104'],
    buyOrders: [10, 15, 20, 25, 30],
    sellOrders: [5, 10, 15, 20, 25],
  });

  // Simulate data fetching/updating
  useEffect(() => {
    const interval = setInterval(() => {
      // Simulate fetching new data
      const newOrderBook = {
        labels: ['100', '101', '102', '103', '104'],
        buyOrders: Array.from({ length: 5 }, () => Math.floor(Math.random() * 30)),
        sellOrders: Array.from({ length: 5 }, () => Math.floor(Math.random() * 30)),
      };
      setOrderBook(newOrderBook);
    }, 5000); // Update data every 5 seconds

    return () => clearInterval(interval);
  }, []);

  const data = {
    labels: orderBook.labels,
    datasets: [
      {
        label: 'Buy Orders',
        data: orderBook.buyOrders,
        backgroundColor: 'rgba(75, 192, 192, 0.2)',
        borderColor: 'rgba(75, 192, 192, 1)',
        borderWidth: 1,
      },
      {
        label: 'Sell Orders',
        data: orderBook.sellOrders,
        backgroundColor: 'rgba(255, 99, 132, 0.2)',
        borderColor: 'rgba(255, 99, 132, 1)',
        borderWidth: 1,
      },
    ],
  };

  const options = {
    scales: {
      x: {
        title: {
          display: true,
          text: 'Price Points',
        },
      },
      y: {
        beginAtZero: true,
        title: {
          display: true,
          text: 'Order Quantity',
        },
      },
    },
  };

  return (
    <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
      <div style={{ width: '60%', height: '60%' }}>
        <Bar data={data} options={options} />
      </div>
    </div>
  );
};

export default LandingPage;
