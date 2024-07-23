CREATE TABLE IF NOT EXISTS Orders (
  id VARCHAR(64) PRIMARY KEY,
  quantity INT,
  price INT,
  placed_at TIMESTAMP,
  order_type CHAR(1) -- B or S
);

CREATE TABLE IF NOT EXISTS Limits (
    price INT,
    limit_type CHAR(1), -- B or S
    quantity INT,
    PRIMARY KEY (price, limit_type)
);