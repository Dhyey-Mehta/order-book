"use client"

import React, { useState, useEffect } from 'react';
import OrderBook from './OrderBook';


const LandingPage = () => {
  return (
    <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
      <div style={{ width: '60%', height: '60%' }}>
        <OrderBook windowWidth={1080}/>
      </div>
    </div>
  );
};

export default LandingPage;
