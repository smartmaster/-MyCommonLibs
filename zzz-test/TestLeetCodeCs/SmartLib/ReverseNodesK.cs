using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestLeetCodeCs.SmartLib
{
    class ReverseNodesK
    {
        public class ListNode
        {
            public int val;
            public ListNode next;
            public ListNode(int x)
            {
                val = x;
            }
        }

        private readonly int _kk;
        private readonly ListNode[] _lst;
        private int _size = 0;

        public ReverseNodesK(int kk)
        {
            _kk = kk;
            _lst = new ListNode[_kk];
        }

        public void PushKNodes(ListNode node)
        {
            int ii = 0;
            for (; ii < _kk; ++ ii)
            {
                if(node != null)
                {
                    _lst[ii] = node;
                    node = node.next;
                }
                else
                {
                    break;
                }
            }
            _size = ii;
        }


        public void Process(ref ListNode newHead, ref ListNode newTail, ref ListNode nextHead)
        {
            if(_size == _kk)
            {
                newHead = _lst[_kk - 1];
                newTail = _lst[0];
                nextHead = newHead.next;
                for(int ii = _kk - 1; ii >= 1; -- ii)
                {
                    _lst[ii].next = _lst[ii - 1];
                }
                newTail.next = nextHead;
            }
            else
            {
                newHead = _lst[0];
                newTail = _lst[_size - 1];
                nextHead = null;
            }
            _size = 0;
        }


        public ListNode ReverseNodesKAPI(ListNode head)
        {
            ListNode newHead = null;
            ListNode newTail = null;
            ListNode nextHead = null;

            PushKNodes(head);
            Process(ref newHead, ref newTail, ref nextHead);
            var result = newHead;
            while(nextHead != null)
            {
                var prevTail = newTail;
                PushKNodes(nextHead);
                Process(ref newHead, ref newTail, ref nextHead);
                prevTail.next = newHead;
            }

            return result;
        }

        public static ListNode API(ListNode head, int kk)
        {
            if(head == null || 1 == kk)
            {
                return head;
            }

            ReverseNodesK rnk = new ReverseNodesK(kk);
            return rnk.ReverseNodesKAPI(head);
        }
    }


}

