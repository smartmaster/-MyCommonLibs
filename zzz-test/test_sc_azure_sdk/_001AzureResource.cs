using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using Microsoft.Azure;
using Microsoft.IdentityModel.Clients.ActiveDirectory;
using Microsoft.Azure.Management.ResourceManager;
using Microsoft.Azure.Management.ResourceManager.Models;
using Microsoft.Azure.Management.Storage;
using Microsoft.Azure.Management.Storage.Models;
using Microsoft.Azure.Management.Network;
using Microsoft.Azure.Management.Network.Models;
using Microsoft.Azure.Management.Compute;
using Microsoft.Azure.Management.Compute.Models;
using Microsoft.Rest;
using Microsoft.Rest.Azure;
using System.Net.Http;

namespace test_sc_azure_sdk
{
    class _001AzureResource
    {
        private string m_application_id = "7fdb9056-a9ae-475f-8b21-5e707f77860e";
        private string m_password = "snR/QgCH0YkRqKjWPgBz9vueXhxowlNxAqvPTXyKQAk=";
        private string m_tenant_id = "1a6cd66d-2cc0-4892-b41e-e01c6f0f8350";

        private string m_groupName = "sssgroupname";
        private string m_ipName = "sssipname";
        private string m_avSetName = "sssavsetname";
        private string m_nicName = "sssnicname";
        private string m_storageName = "sssstoragename";
        private string m_vmName = "sssvmname";
        private string m_vnetName = "sssvnetname";
        private string m_subnetName = "ssssubnetname";
        private string m_adminName = "sss";
        private string m_adminPassword = "sssC@1234567";
        private string m_location = "eastus";
        private string m_subscriptionId = "7315bf64-0ed9-46d8-b4e6-1a6082194d22";

        private string m_existingResourceGroup = "hbbu-res-group";
        private string m_existingVmName = "hbbu-vm";

        private TokenCredentials m_credential = null;
        
        public async Task<string> GetAuthorizationHeaderAsync()
        {
            ClientCredential cc = new ClientCredential($"{m_application_id}", $"{m_password}");
            var context = new AuthenticationContext($"https://login.windows.net/{m_tenant_id}");
            var result = await context.AcquireTokenAsync("https://management.azure.com/", cc);

            if (result == null)
            {
                throw new InvalidOperationException("Failed to obtain the JWT token");
            }

            string token = result.AccessToken;
            m_credential = new TokenCredentials(token);

            return token;
        }


        public async Task<IEnumerable<Location>> GetLocationsAsync()
        {
            var client = new SubscriptionClient(m_credential);
            IEnumerable<Location> locations = await client.Subscriptions.ListLocationsAsync(m_subscriptionId);
            foreach(var loc in locations)
            {
                Console.WriteLine($"{loc.Name}, {loc.DisplayName}");
            }
            return locations;
        }

        public async Task<IPage<ResourceGroup>> GetResourceGroups()
        {
            var client = new ResourceManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };
            IPage<ResourceGroup> resourceGroups = await client.ResourceGroups.ListAsync();
            return resourceGroups;
        }

        public async Task<IEnumerable<VirtualMachine>> GetVirtualMachinesAsync(string resouceGroup)
        {
            var client = new ComputeManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };

            IEnumerable<VirtualMachine> vms = await client.VirtualMachines.ListAsync(resouceGroup);
            return vms;
        }


        public async Task<IEnumerable<StorageAccount>> GetStorageAccountsAsync(string resouceGroup)
        {
            var client = new StorageManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };
            IEnumerable<StorageAccount> storAccounts = await client.StorageAccounts.ListByResourceGroupAsync(resouceGroup);
            return storAccounts;
        }


        public async Task GetVirtualMachineImageResourcesAsync(string publisherFilter)
        {
            var client = new ComputeManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };

            IList<VirtualMachineImageResource> publishers = await client.VirtualMachineImages.ListPublishersAsync(m_location);
            foreach (var publisher in publishers)
            {
                Console.WriteLine(publisher.Name);
                if(string.IsNullOrEmpty(publisherFilter) || 0 == string.Compare(publisher.Name, publisherFilter, true))
                {
                    IList<VirtualMachineImageResource> offers = await client.VirtualMachineImages.ListOffersAsync(m_location, publisher.Name);
                    foreach (var offer in offers)
                    {
                        Console.WriteLine($"\t{publisher.Name}, {offer.Name}");
                        IList<VirtualMachineImageResource> skus = await client.VirtualMachineImages.ListSkusAsync(m_location, publisher.Name, offer.Name);
                        foreach (var sku in skus)
                        {
                            Console.WriteLine($"\t\t{publisher.Name}, {offer.Name}, {sku.Name}");
                        }

                    }
                }
               
            }
        }

        public async Task GetSubscriptionsAsync()
        {
            SubscriptionClient client = new SubscriptionClient(m_credential);
            IPage<Subscription> subscriptions = await client.Subscriptions.ListAsync();
            foreach (var sub in subscriptions)
            {
                Console.WriteLine($"{nameof(sub.DisplayName)}={sub.DisplayName}");
                Console.WriteLine($"{nameof(sub.Id)}={sub.Id}");
                Console.WriteLine($"{nameof(sub.State)}={sub.State}");
                Console.WriteLine($"{nameof(sub.SubscriptionId)}={sub.SubscriptionId}");
                Console.WriteLine();
            }
        }

        public async Task GetVirtualMachineSizesAsync()
        {
            var client = new ComputeManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };
            var vmSizes = await client.VirtualMachineSizes.ListAsync(m_location);
            foreach (var vmSize in vmSizes)
            {
                Console.WriteLine($"{nameof(vmSize.Name)}={vmSize.Name}");
                Console.WriteLine($"{nameof(vmSize.MaxDataDiskCount)}={vmSize.MaxDataDiskCount}");
                Console.WriteLine($"{nameof(vmSize.MemoryInMB)}={vmSize.MemoryInMB}");
                Console.WriteLine($"{nameof(vmSize.NumberOfCores)}={vmSize.NumberOfCores}");
                Console.WriteLine($"{nameof(vmSize.OsDiskSizeInMB)}={vmSize.OsDiskSizeInMB}");
                Console.WriteLine($"{nameof(vmSize.ResourceDiskSizeInMB)}={vmSize.ResourceDiskSizeInMB}");
                Console.WriteLine();
            }
        }


        public async Task<VirtualMachine> GetVirtualMachineAsync(string resourceGroup, string vmName)
        {
            var client = new ComputeManagementClient(m_credential)
            {
                SubscriptionId = m_subscriptionId
            };

            var vm = await client.VirtualMachines.GetAsync(resourceGroup, vmName);
            return vm;
        }

        public async Task CreateResourceGroupAsync(
        )
        {
            Console.WriteLine("Creating the resource group...");
            var resourceManagementClient = new ResourceManagementClient(m_credential);
            resourceManagementClient.SubscriptionId = m_subscriptionId;
            var resourceGroup = new ResourceGroup
            {
                Location = m_location
            };
            var rgResult = await resourceManagementClient.ResourceGroups.CreateOrUpdateAsync(m_groupName, resourceGroup);
            Console.WriteLine(rgResult.Properties.ProvisioningState);

            const string Microsoft_Storage = "Microsoft.Storage";
            var rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Storage);
            Console.WriteLine($"{Microsoft_Storage} {rpResult.RegistrationState}");

            const string Microsoft_Network = "Microsoft.Network";
            rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Network);
            Console.WriteLine($"{Microsoft_Network} {rpResult.RegistrationState}");


            const string Microsoft_Compute = "Microsoft.Compute";
            rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Compute);
            Console.WriteLine($"{Microsoft_Compute} {rpResult.RegistrationState}");
        }


        public async Task CreateStorageAccountAsync(
        )
        {
            Console.WriteLine("Creating the storage account...");
            var storageManagementClient = new StorageManagementClient(m_credential);
            storageManagementClient.SubscriptionId = m_subscriptionId;
            var saResult = await storageManagementClient.StorageAccounts.CreateAsync(
            m_groupName,
            m_storageName,
            new StorageAccountCreateParameters()
            {
                //AccountType = AccountType.StandardLRS,
                Sku = new Microsoft.Azure.Management.Storage.Models.Sku(SkuName.StandardLRS),
                Location = m_location
            }
            );

            var storKeys = await storageManagementClient.StorageAccounts.ListKeysAsync(m_groupName, m_storageName);
            foreach (var key in storKeys.Keys)
            {
                Console.WriteLine($"{nameof(key.KeyName)}={key.KeyName}");
                Console.WriteLine($"{nameof(key.Value)}={key.Value}");
                Console.WriteLine();
            }

            var storAcc = await storageManagementClient.StorageAccounts.GetPropertiesAsync(m_groupName, m_storageName);

            Console.WriteLine(saResult.ProvisioningState);
        }

        public async Task CreatePublicIPAddress(
        )
        {
            Console.WriteLine("Creating the public ip...");
            var networkManagementClient = new NetworkManagementClient(m_credential);
            networkManagementClient.SubscriptionId = m_subscriptionId;
            var ipResult = await networkManagementClient.PublicIPAddresses.CreateOrUpdateAsync(
            m_groupName,
            m_ipName,
            new PublicIPAddress
            {
                Location = m_location,
                PublicIPAllocationMethod = "Dynamic"
            }
            );
            Console.WriteLine(ipResult.ProvisioningState);
        }

        public async Task CreateNetworkAsync(
            )
        {
            Console.WriteLine("Creating the virtual network...");
            var networkManagementClient = new NetworkManagementClient(m_credential);
            networkManagementClient.SubscriptionId = m_subscriptionId;

            var subnet = new Subnet
            {
                Name = m_subnetName,
                AddressPrefix = "10.0.0.0/24"
            };

            var address = new AddressSpace
            {
                AddressPrefixes = new List<string> { "10.0.0.0/16" }
            };

            var vnResult = await networkManagementClient.VirtualNetworks.CreateOrUpdateAsync(
            m_groupName,
            m_vnetName,
            new VirtualNetwork
            {
                Location = m_location,
                AddressSpace = address,
                Subnets = new List<Subnet> { subnet }
            }
            );

            Console.WriteLine(vnResult.ProvisioningState);

            var subnetResponse = await networkManagementClient.Subnets.GetAsync(
            m_groupName,
            m_vnetName,
            m_subnetName
            );

            var pubipResponse = await networkManagementClient.PublicIPAddresses.GetAsync(m_groupName, m_ipName);

            Console.WriteLine("Updating the network with the nic...");
            var nicResult = await networkManagementClient.NetworkInterfaces.CreateOrUpdateAsync(
            m_groupName,
            m_nicName,
            new NetworkInterface
            {
                Location = m_location,
                IpConfigurations = new List<NetworkInterfaceIPConfiguration>
                {
                    new NetworkInterfaceIPConfiguration
                    {
                        Name = "nicConfig1",
                        PublicIPAddress = pubipResponse,
                        Subnet = subnetResponse
                    }
                }
            }
            );
            Console.WriteLine(vnResult.ProvisioningState);
        }


        public async Task CreateAvailabilitySetAsync(
            )
        {
            Console.WriteLine("Creating the availability set...");
            var computeManagementClient = new ComputeManagementClient(m_credential);
            computeManagementClient.SubscriptionId = m_subscriptionId;
            var avResult = await computeManagementClient.AvailabilitySets.CreateOrUpdateAsync(
            m_groupName,
            m_avSetName,
            new AvailabilitySet()
            {
                Location = m_location
            }
            );
        }


        public async Task CreateVirtualMachineAsync(
            )
        {
            var networkManagementClient = new NetworkManagementClient(m_credential);
            networkManagementClient.SubscriptionId = m_subscriptionId;
            var nic = await networkManagementClient.NetworkInterfaces.GetAsync(m_groupName, m_nicName);

            var computeManagementClient = new ComputeManagementClient(m_credential);
            computeManagementClient.SubscriptionId = m_subscriptionId;
            var avSet = await computeManagementClient.AvailabilitySets.GetAsync(m_groupName, m_avSetName);
            

            Console.WriteLine("Creating the virtual machine...");

            bool bCreateNewDataDisk = true;
            bool bAttcheExisting = true;
            var vm = await computeManagementClient.VirtualMachines.CreateOrUpdateAsync(
            m_groupName,
            m_vmName,
            new VirtualMachine
            {
                Location = m_location,
                AvailabilitySet = new Microsoft.Azure.Management.Compute.Models.SubResource
                {
                    Id = avSet.Id
                },
                HardwareProfile = new HardwareProfile
                {
                    //VmSize = "Standard_A0"
                    VmSize = "Standard_DS1_V2"
                },
                OsProfile = new OSProfile
                {
                    AdminUsername = m_adminName,
                    AdminPassword = m_adminPassword,
                    ComputerName = m_vmName,
                    WindowsConfiguration = new WindowsConfiguration
                    {
                        ProvisionVMAgent = true
                    }
                },
                NetworkProfile = new NetworkProfile
                {
                    NetworkInterfaces = new List<NetworkInterfaceReference>
                    {
                        new NetworkInterfaceReference { Id = nic.Id }
                    }
                },

                StorageProfile = new StorageProfile
                {
                    ImageReference = new ImageReference
                    {
                        Publisher = "MicrosoftWindowsServer",
                        Offer = "WindowsServer",
                        Sku = "2012-R2-Datacenter",
                        Version = "latest"
                    },
                    OsDisk = new OSDisk
                    {
                        Name = "mytestod1",
                        CreateOption = DiskCreateOptionTypes.FromImage,
                        Vhd = new VirtualHardDisk
                        {
                            Uri = "https://" + m_storageName + ".blob.core.windows.net/vhds/mytestod1.vhd"
                        }
                    },
                    DataDisks = bCreateNewDataDisk ? new List<DataDisk>()
                    {
                        new DataDisk()
                        {
                            Caching = CachingTypes.ReadOnly,
                            CreateOption = bAttcheExisting? DiskCreateOptionTypes.Attach : DiskCreateOptionTypes.Empty,
                            DiskSizeGB = 1023,
                            Lun = 0,
                            Name = "sssDataDisk0",
                            Vhd = new VirtualHardDisk()
                            {
                                Uri = "https://" + m_storageName + ".blob.core.windows.net/vhds/sssDataDisk0.vhd"
                            }
                        },


                        new DataDisk()
                        {
                            Caching = CachingTypes.ReadOnly,
                            CreateOption = bAttcheExisting? DiskCreateOptionTypes.Attach : DiskCreateOptionTypes.Empty,
                            DiskSizeGB = 1023,
                            Lun = 1,
                            Name = "sssDataDisk1",
                            Vhd = new VirtualHardDisk()
                            {
                                Uri = "https://" + m_storageName + ".blob.core.windows.net/vhds/sssDataDisk1.vhd"
                            }
                        }
                    } :
                    new List<DataDisk>()
                }
            }
            );
            Console.WriteLine(vm.ProvisioningState);
        }


        public async Task PowerOnOffVirtualMachineAsync(
            )
        {
            ComputeManagementClient client = new ComputeManagementClient(m_credential);
            client.SubscriptionId = m_subscriptionId;

            VirtualMachine vm = await client.VirtualMachines.GetAsync(m_groupName, m_vmName);
            Console.WriteLine("Power Off VM...");
            await client.VirtualMachines.PowerOffAsync(m_groupName, m_vmName);
            Console.WriteLine("Start VM...");
            await client.VirtualMachines.StartAsync(m_groupName, m_vmName);
            Console.WriteLine("Power Off VM...");
            await client.VirtualMachines.PowerOffAsync(m_groupName, m_vmName);
        }

        public async Task DeleteResourceGroupAsync(
            )
        {
            Console.WriteLine("Deleting resource group...");
            var resourceGroupClient = new ResourceManagementClient(m_credential);
            resourceGroupClient.SubscriptionId = m_subscriptionId;
            await resourceGroupClient.ResourceGroups.DeleteAsync(m_groupName);
        }

        public async Task TestAzureOperationsAsync()
        {
            try
            {

                string token = await GetAuthorizationHeaderAsync();
                Console.WriteLine($"{nameof(token)}={token}");


                await GetLocationsAsync();
                

                await GetSubscriptionsAsync();

                await GetVirtualMachineImageResourcesAsync("Microsoft");

                IPage<ResourceGroup> resourceGroups = await GetResourceGroups();
                foreach(var resGroup in resourceGroups)
                {
                    IEnumerable<VirtualMachine> vms = await GetVirtualMachinesAsync(resGroup.Name);
                    foreach (var tempVM in vms)
                    {
                        Console.WriteLine($"{tempVM.Name}");
                    }


                    IEnumerable<StorageAccount> storAccoutns = await GetStorageAccountsAsync(resGroup.Name);
                    foreach (var storAcc in storAccoutns)
                    {
                        Console.WriteLine($"{storAcc.Name}");
                    }
                }

                await GetVirtualMachineSizesAsync();

                var vm = await GetVirtualMachineAsync(m_existingResourceGroup, m_existingVmName);
                Console.WriteLine($"{nameof(vm.Name)}={vm.Name}");

                await CreateResourceGroupAsync(
                );
                
                await CreateStorageAccountAsync(
                );

                await CreatePublicIPAddress(
                );

                await CreateNetworkAsync(
                );


                await CreateAvailabilitySetAsync(
                );

                await CreateVirtualMachineAsync(
                );

                await PowerOnOffVirtualMachineAsync(
                );

                bool toDeleteResource = false;
                if (toDeleteResource)
                {
                    await DeleteResourceGroupAsync(
                          );
                }

            }
            catch (System.Exception ex)
            {
                Console.WriteLine($"{ex.Message}");
                Console.WriteLine($"{ex}");
                throw;
            }
        }
    }


    static class _001AzureResourceTest
    {
        public static void TestCases(int testCase)
        {
            switch (testCase)
            {
                case 1:
                    {
                        var azureRM = new _001AzureResource();
                        Task task = azureRM.TestAzureOperationsAsync();
                        task.Wait();
                    }
                    break;
                case 0:
                    {
                        var azureRM = new _001AzureResource();
                        Task<string> task = azureRM.GetAuthorizationHeaderAsync();
                        task.Wait();
                        string token = task.Result;
                        Console.WriteLine($"{nameof(token)}={token}");
                    }
                    break;
                default:
                    {
                        throw new InvalidOperationException("invalid test case number");
                    }


            }
        }
    }
}

