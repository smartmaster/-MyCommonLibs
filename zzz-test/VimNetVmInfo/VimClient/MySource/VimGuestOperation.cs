
using System;
using System.Net;
using System.Text;
using System.IO;
using VimLib.VimServiceReference;

namespace VimClient
{


    interface IProcessGuestOperation
    {
        GuestProcessInfo[] ListProcessesInGuest(long[] pids);
        string[] ReadEnvironmentVariableInGuest(string[] names);
        long StartProgramInGuest(GuestProgramSpec guestProgramSpec);
        void TerminateProcessInGuest(long pid);
    }

    interface IFileGuestOperation
    {
        //void ChangeFileAttributesInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string guestFilePath, VimLib.VimServiceReference.GuestFileAttributes fileAttributes);
        void ChangeFileAttributesInGuest(string guestFilePath, GuestFileAttributes fileAttributes);
        //public string CreateTemporaryDirectoryInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string prefix, string suffix, string directoryPath)
        string CreateTemporaryDirectoryInGuest(string prefix, string suffix, string directoryPath);
        //string CreateTemporaryFileInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string prefix, string suffix, string directoryPath)
        string CreateTemporaryFileInGuest(string prefix, string suffix, string directoryPath);
        //public void DeleteDirectoryInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string directoryPath, bool recursive)
        void DeleteDirectoryInGuest(string directoryPath, bool recursive);
        //public void DeleteFileInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string filePath)
        void DeleteFileInGuest(string filePath);
        //public VimLib.VimServiceReference.FileTransferInformation InitiateFileTransferFromGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string guestFilePath)
        void FileTransferFromGuest(string guestFilePath, string localFilePath);
        //public string InitiateFileTransferToGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string guestFilePath, VimLib.VimServiceReference.GuestFileAttributes fileAttributes, long fileSize, bool overwrite)
        void FileTransferToGuest(string guestFilePath, string localFilePath, bool overwrite);
        //public VimLib.VimServiceReference.GuestListFileInfo ListFilesInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string filePath, int index, int maxResults, string matchPattern)
        GuestListFileInfo ListFilesInGuest(string filePath, int index, int maxResults, string matchPattern);
        //void MakeDirectoryInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string directoryPath, bool createParentDirectories);
        void MakeDirectoryInGuest(string directoryPath, bool createParentDirectories);
        //void MoveDirectoryInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string srcDirectoryPath, string dstDirectoryPath);
        void MoveDirectoryInGuest(string srcDirectoryPath, string dstDirectoryPath);
        //        void MoveFileInGuest(VimLib.VimServiceReference.ManagedObjectReference _this, VimLib.VimServiceReference.ManagedObjectReference vm, VimLib.VimServiceReference.GuestAuthentication auth, string srcFilePath, string dstFilePath, bool overwrite);
        void MoveFileInGuest(string srcFilePath, string dstFilePath, bool overwrite);
    }



    class VimGuestOperation : IProcessGuestOperation, IFileGuestOperation
    {
        private readonly VimPortTypeClient _vimService;
        private readonly ManagedObjectReference _morPropCollector;
        private readonly ManagedObjectReference _morGuestOperationsManager;
        private readonly ManagedObjectReference _morVM;
        //private readonly string _user;
        //private readonly string _password;

        private readonly ManagedObjectReference _morFileManager;
        private readonly ManagedObjectReference _morProcessManager;

        private readonly NamePasswordAuthentication _NamePasswordAuthentication;


        public VimGuestOperation(
            VimPortTypeClient vimService,
            ManagedObjectReference morPropCollector,
            ManagedObjectReference morGuestOperationsManager,
            ManagedObjectReference morVM,
            string user,
            string pwd
            )
        {
            _vimService = vimService;
            _morPropCollector = morPropCollector;
            _morGuestOperationsManager = morGuestOperationsManager;
            _morVM = morVM;
            //_user = user;
            //_password = password;

            _NamePasswordAuthentication = new NamePasswordAuthentication()
            {
                interactiveSession = false,
                username = user,
                password = pwd
            };

            VimPropCollector pc = new VimPropCollector(_vimService, _morPropCollector);
            _morFileManager = (ManagedObjectReference)pc.Get(morGuestOperationsManager, "fileManager");
            _morProcessManager = (ManagedObjectReference)pc.Get(morGuestOperationsManager, "processManager");
        }

        public GuestProcessInfo[] ListProcessesInGuest(long[] pids)
        {
            //throw new NotImplementedException();
            return _vimService.ListProcessesInGuest(
                _morProcessManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                pids//long[] pids
                );
        }

        public string[] ReadEnvironmentVariableInGuest(string[] names)
        {
            //throw new NotImplementedException();
            return _vimService.ReadEnvironmentVariableInGuest(
                _morProcessManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                names//string[] names
                );
        }

        public long StartProgramInGuest(GuestProgramSpec guestProgramSpec)
        {
            //throw new NotImplementedException();
            return _vimService.StartProgramInGuest(
                _morProcessManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                guestProgramSpec//VimLib.VimServiceReference.GuestProgramSpec spec
                );
        }

        public void TerminateProcessInGuest(long pid)
        {
            //throw new NotImplementedException();
            _vimService.TerminateProcessInGuest(
                _morProcessManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                pid//long pid
                );
        }

        public void ChangeFileAttributesInGuest(string guestFilePath, GuestFileAttributes fileAttributes)
        {
            //throw new NotImplementedException();
            _vimService.ChangeFileAttributesInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                guestFilePath,//string guestFilePath, 
                fileAttributes//VimLib.VimServiceReference.GuestFileAttributes fileAttributes
                );
        }

        public string CreateTemporaryDirectoryInGuest(string prefix, string suffix, string directoryPath)
        {
            //throw new NotImplementedException();
            return _vimService.CreateTemporaryDirectoryInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                prefix, suffix, directoryPath
                );
        }

        public string CreateTemporaryFileInGuest(string prefix, string suffix, string directoryPath)
        {
            //throw new NotImplementedException();
            return _vimService.CreateTemporaryFileInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                prefix, suffix, directoryPath
                );
        }

        public void DeleteDirectoryInGuest(string directoryPath, bool recursive)
        {
            //throw new NotImplementedException();
            _vimService.DeleteDirectoryInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                directoryPath, recursive
                );
        }

        public void DeleteFileInGuest(string filePath)
        {
            //throw new NotImplementedException();
            _vimService.DeleteFileInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                filePath
                );
        }

        public void FileTransferFromGuest(string guestFilePath, string localFilePath)
        {
            //throw new NotImplementedException();
            FileTransferInformation fti = _vimService.InitiateFileTransferFromGuest(
                 _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                 _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                 _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                 guestFilePath
                 );
            WebClient webClient = new WebClient();
            webClient.DownloadFile(fti.url, localFilePath);
        }

        public void FileTransferToGuest(string guestFilePath, string localFilePath, bool overwrite)
        {
            //throw new NotImplementedException();
            //var fileAttributes = new GuestWindowsFileAttributes()
            //{
            //    accessTime = System.DateTime.Now,
            //    modificationTime = System.DateTime.Now
            //};

            GuestWindowsFileAttributes fileAttributes = new GuestWindowsFileAttributes();

            System.IO.FileInfo fi = new System.IO.FileInfo(localFilePath);

            string fileUploadUrl = _vimService.InitiateFileTransferToGuest(
                 _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                 _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                 _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                 guestFilePath, fileAttributes, fi.Length, overwrite
                 );

            WebClient webClient = new WebClient();
            byte[] response = webClient.UploadFile(fileUploadUrl, "PUT", localFilePath);
            string responseText = Encoding.UTF8.GetString(response);
        }

        public GuestListFileInfo ListFilesInGuest(string filePath, int index, int maxResults, string matchPattern)
        {
            //throw new NotImplementedException();
            return _vimService.ListFilesInGuest(
                 _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                 _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                 _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                 filePath, index, maxResults, matchPattern
                 );
        }

        public void MakeDirectoryInGuest(string directoryPath, bool createParentDirectories)
        {
            //throw new NotImplementedException();
            _vimService.MakeDirectoryInGuest(
                 _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                 _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                 _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                 directoryPath, createParentDirectories
                 );
        }

        public void MoveDirectoryInGuest(string srcDirectoryPath, string dstDirectoryPath)
        {
            //throw new NotImplementedException();
            _vimService.MoveDirectoryInGuest(
                _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                srcDirectoryPath, dstDirectoryPath
                );
        }

        public void MoveFileInGuest(string srcFilePath, string dstFilePath, bool overwrite)
        {
            //throw new NotImplementedException();
            _vimService.MoveFileInGuest(
                  _morFileManager,//VimLib.VimServiceReference.ManagedObjectReference _this, 
                  _morVM,//VimLib.VimServiceReference.ManagedObjectReference vm, 
                  _NamePasswordAuthentication,//VimLib.VimServiceReference.GuestAuthentication auth, 
                  srcFilePath, dstFilePath, overwrite
                  );
        }
    }
}