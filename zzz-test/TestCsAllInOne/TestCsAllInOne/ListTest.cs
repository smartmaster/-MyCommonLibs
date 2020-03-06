using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{

    public class ListNode
    {
        public int val;
        public ListNode next;
        public ListNode(int x) { val = x; }
    }


    class ListTest7Java
    {
        class Result
        {
            public ListNode newHead;
            public ListNode efirst;
            public ListNode elast;

            public Result(ListNode nn1, ListNode nn2, ListNode nn3)
            {
                newHead = nn1;
                efirst = nn2;
                elast = nn3;
            }
        }
        Result Partition(ListNode head, ListNode stop)
        {
            int pivot = head.val;
            ListNode cur = head;


            ListNode smaller = new ListNode(0);
            ListNode equal = new ListNode(0);
            ListNode larger = new ListNode(0);

            ListNode curSmaller = smaller;
            ListNode curLarger = larger;
            ListNode curEqual = equal;

            while (cur != stop)
            {
                if (cur.val < pivot)
                {
                    curSmaller.next = cur;
                    curSmaller = cur;
                }
                else if (cur.val == pivot)
                {
                    curEqual.next = cur;
                    curEqual = cur;
                }
                else
                {
                    curLarger.next = cur;
                    curLarger = cur;
                }
                cur = cur.next;
            }

            curSmaller.next = equal.next;
            if (null != larger.next)
            {
                curEqual.next = larger.next;
                curLarger.next = stop;
            }
            else
            {
                curEqual.next = stop;
            }


            return new Result(smaller.next, equal.next, curEqual);
        }

        ListNode QuickSort(ListNode head, ListNode stop)
        {
            if (null == head || stop == head)
            {
                return null;
            }

            if (stop == head.next)
            {
                return head;
            }


            Result rr = Partition(head, stop);


            ListNode left = QuickSort(rr.newHead, rr.efirst);
            if (null == left)
            {
                left = rr.newHead;
            }

            ListNode right = QuickSort(rr.elast.next, stop);
            if (null != right)
            {
                rr.elast.next = right;
            }
            else
            {
                rr.elast.next = stop;
            }
            return left;
        }


        public ListNode SortList(ListNode head)
        {
            return QuickSort(head, null);
        }

    };

    class ListTest6
    {
		static ListNode Partition(ListNode head, ListNode stop, out ListNode efirst, out ListNode elast)
        {
            int pivot = head.val;
            ListNode cur = head;


            ListNode smaller = new ListNode(0);
            ListNode equal = new ListNode(0);
            ListNode larger = new ListNode(0);

            ListNode curSmaller = smaller;
            ListNode curLarger = larger;
            ListNode curEqual = equal;

            while (cur != stop)
            {
                if (cur.val < pivot)
                {
                    curSmaller.next = cur;
                    curSmaller = cur;
                }
                else if (cur.val == pivot)
                {
                    curEqual.next = cur;
                    curEqual = cur;
                }
                else
                {
                    curLarger.next = cur;
                    curLarger = cur;
                }
                cur = cur.next;
            }

            curSmaller.next = equal.next;
            if (null != larger.next)
            {
                curEqual.next = larger.next;
                curLarger.next = stop;
            }
            else
            {
                curEqual.next = stop;
            }


            efirst = equal.next;
            elast = curEqual;

            return smaller.next;
        }

        static ListNode QuickSort(ListNode head, ListNode stop)
        {
            if (null == head || stop == head)
            {
                return null;
            }

            if (stop == head.next)
            {
                return head;
            }


            ListNode efirst = null;
            ListNode elast = null;
            ListNode newHead = Partition(head, stop, out efirst, out elast);


            ListNode left = QuickSort(newHead, efirst);
            if (null == left)
            {
                left = newHead;
            }

            ListNode right = QuickSort(elast.next, stop);
            if (null != right)
            {
                elast.next = right;
            }
            else
            {
                elast.next = stop;
            }
            return left;
        }


        public ListNode SortList(ListNode head)
        {
            return QuickSort(head, null);
        }
  
    };
    class ListTest
    {
        //https://leetcode.com/problems/sort-list/
        //Sort a linked list in O(n log n) time using constant space complexity.

        struct Pair3
        {
            public ListNode p1;
            public ListNode p2;
            public ListNode p3;

            public Pair3(ListNode p1_, ListNode p2_, ListNode p3_)
            {
                p1 = p1_;
                p2 = p2_;
                p3 = p3_;
            }

            public void MoveNext()
            {
                p1 = p2;
                p2 = p3;
                p3 = p3?.next;
            }
        }



        static Pair3 FindLarger(int val, ListNode first, ListNode stop)
        {
            Pair3 ppp = new Pair3(null, null, first);

            while (ppp.p3 != stop)
            {
                if (ppp.p3.val > val)
                {
                    break;
                }
                ppp.MoveNext();
            }
            return ppp;
        }


        static Pair3 FindEqualSmaller(int val, ListNode first, ListNode stop)
        {
            Pair3 ppp = new Pair3(null, null, first);

            while (ppp.p3 != stop)
            {
                if (ppp.p3.val <= val)
                {
                    break;
                }
                ppp.MoveNext();
            }

            return ppp;
        }
                          
        static Pair3 Partition(ListNode first, ListNode stop)
        {
            int val = first.val;
            Pair3 larger = FindLarger(val, first, stop);
            Pair3 smaller = FindEqualSmaller(val, larger.p3, stop);

            while (smaller.p3 != stop)
            {
                ref int aa = ref larger.p3.val;
                ref int bb = ref smaller.p3.val;
                int temp = aa;
                aa = bb;
                bb = temp;
                larger.MoveNext();
                smaller = FindEqualSmaller(val, smaller.p3, stop);
            }

            if (larger.p2 != first)
            {
                ref int aa = ref larger.p2.val;
                ref int bb = ref first.val;
                int temp = aa;
                aa = bb;
                bb = temp;
            }

            return larger;
        }

        static void QuickSort(ListNode first, ListNode stop)
        {
            var ppp = Partition(first, stop);

            if (ppp.p1 != null)
            {
                QuickSort(first, ppp.p2);
            }

            if (ppp.p3 != null && ppp.p3 != stop)
            {
                QuickSort(ppp.p3, stop);
            }

        }

        public /*static*/ ListNode SortList(ListNode head)
        {
            if (null == head || null == head.next)
            {
                return head;
            }

            QuickSort(head, null);
            return head;
        }

    }
}
