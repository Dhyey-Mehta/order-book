import { Server } from 'socket.io'
import { Kafka } from 'kafkajs';


const SocketHandler = async (req, res) => {
    const kafka = new Kafka({
        clientId: 'order-book-client',
        brokers: [/* kafka brokers here*/],
        ssl: true,
        sasl: {
            mechanism: 'plain',
            username: "kafka_api_key",
            password: "kafka_api_key",
        },
    });

    const consumer = kafka.consumer({ groupId: 'order-book-client' });
    await consumer.subscribe({ topic: 'trades', fromBeginning: true });
    await consumer.subscribe({ topic: 'matches', fromBeginning: true });

    const format_order_message = (msg: string) => {
        const parts = msg.split(',');

        const id = parts[0];
        const cust_id = parts[1];
        const price = parseFloat(parts[2]);
        const quantity = parseInt(parts[3]);
        const type = parts[4] === '0' ? 'bids' : 'asks';

        return {
            id: id,
            cust_id: cust_id,
            price: price,
            quantity: quantity,
            type: type
        };
    }

    const format_match_message = (msg: string) => {
        const parts = msg.split(',');

        const order1_id = parts[0];
        const order2_id = parts[1];
        const price = parseFloat(parts[2]);
        const quantity = parseInt(parts[3]);
        const ask_price = parseFloat(parts[4]);
        const bid_price = parseFloat(parts[5]);

        return {
            order1_id: order1_id,
            order2_id: order2_id,
            price: price,
            quantity: quantity,
            ask_price: ask_price,
            bid_price: bid_price
        };
    }

    if (res.socket.server.io) {
        console.log('Socket is already running')
    } else {
        console.log('Socket is initializing')
        const io = new Server(res.socket.server)
        res.socket.server.io = io
        io.on('connection', async (socket) => {
            await consumer.run({
                eachMessage: async ({ topic, partition, message }) => {
                    if (topic == 'matches') {
                        // @ts-ignore
                        socket.broadcast.emit('match', format_match_message(message.value?.toString()))        
                    } else if (topic == 'trades')
                        // @ts-ignore
                        socket.broadcast.emit('trade', format_order_message(message.value?.toString()))
                },
            });
        })
    }
    res.end()
}

export default SocketHandler