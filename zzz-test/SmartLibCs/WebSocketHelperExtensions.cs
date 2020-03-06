using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Net.WebSockets;

using Newtonsoft.Json;
using System.Threading;
using System.Text;
using System.IO;

namespace SmartLib
{

    public static class WebSocketHelperExtensions
    {
        public static async Task SendObject(this WebSocket ws, object obj, CancellationToken ct = default(CancellationToken))
        {
            string str = JsonConvert.SerializeObject(obj, Formatting.Indented);
            await ws.SendString(str, ct);
        }


        public static async Task<T> ReceiveObject<T>(this WebSocket ws, byte[] helperBuffer = null, CancellationToken ct = default(CancellationToken))
        {
            string str = await ws.ReceiveString(helperBuffer, ct);
            return JsonConvert.DeserializeObject<T>(str);
        }

        public static async Task SendString(this WebSocket ws, string str, CancellationToken ct = default(CancellationToken))
        {
            byte[] buffer = Encoding.UTF8.GetBytes(str);
            ArraySegment<byte> seg = new ArraySegment<byte>(buffer);
            await ws.SendAsync(seg, WebSocketMessageType.Text, true, ct);
        }


        public static async Task<string> ReceiveString(this WebSocket ws, byte[] helperBuffer = null, CancellationToken ct = default(CancellationToken))
        {
            const int BUFFER_SIZE = 16 * 1024;
            if (helperBuffer == null)
            {
                helperBuffer = new byte[BUFFER_SIZE];
            }

            using (var bd = new BatchDisposable())
            {
                bool closed = false;
                WebSocketReceiveResult rr = null;
                MemoryStream ms = new MemoryStream();
                bd.Add(ms); //cleanup
                for (;;)
                {
                    ArraySegment<byte> seg = new ArraySegment<byte>(helperBuffer);
                    rr = await ws.ReceiveAsync(seg, ct);
                    await ms.WriteAsync(seg.Array, 0, rr.Count);
                    if (rr.MessageType == WebSocketMessageType.Close)
                    {
                        closed = true;
                        break;
                    }
                    if (rr.EndOfMessage)
                    {
                        break;
                    }
                }

                if (closed)
                {
                    return string.Empty;
                }
                else
                {
                    ms.Seek(0, SeekOrigin.Begin);
                    StreamReader reader = new StreamReader(ms, Encoding.UTF8, false, BUFFER_SIZE, true);
                    bd.Add(reader); //cleanup

                    return await reader.ReadToEndAsync();
                }
               
            }
        }


        public static async Task SendBuffer(this WebSocket ws, byte[] buffer, CancellationToken ct = default(CancellationToken))
        {
            ArraySegment<byte> seg = new ArraySegment<byte>(buffer);
            await ws.SendAsync(seg, WebSocketMessageType.Binary, true, ct);
        }


        public static async Task<int> ReceiveBuffer(this WebSocket ws, MemoryStream ms, byte[] helperBuffer = null, CancellationToken ct = default(CancellationToken))
        {
            if (helperBuffer == null)
            {
                helperBuffer = new byte[1024 * 1024];
            }

            int cout = 0;
            ArraySegment<byte> seg = new ArraySegment<byte>(helperBuffer);

            ms.Seek(0, SeekOrigin.Begin);

            WebSocketReceiveResult rr = null;
            bool closed = false;
            for (;;)
            {
                rr = await ws.ReceiveAsync(seg, ct);
                cout += rr.Count;
                await ms.WriteAsync(helperBuffer, 0, rr.Count);
                if (rr.MessageType == WebSocketMessageType.Close)
                {
                    closed = true;
                    break;
                }
                if (rr.EndOfMessage)
                {
                    break;
                }
            }

            ms.Seek(0, SeekOrigin.Begin);
            return closed? 0 : cout;
        }
    }
}
