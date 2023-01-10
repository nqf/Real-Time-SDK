/*|-----------------------------------------------------------------------------
 *|            This source code is provided under the Apache 2.0 license      --
 *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
 *|                See the project's LICENSE.md for details.                  --
 *|           Copyright (C) 2022-2023 Refinitiv. All rights reserved.         --
 *|-----------------------------------------------------------------------------
 */

using LSEG.Eta.Common;
using System.Diagnostics;
using Buffer = LSEG.Eta.Codec.Buffer;

namespace LSEG.Eta.ValueAdd.Reactor
{
    /// <summary>
    /// This class represents the OAuth credential renewal information.
    /// </summary>
    public class ReactorOAuthCredentialRenewal
    {
        private Buffer m_ClientId = new Buffer();
        private Buffer m_ClientSecret = new Buffer();
        private Buffer m_TokenScope = new Buffer();
        private Buffer m_ClientJwk = new Buffer();

        /// <summary>
        /// Creates <see cref="ReactorOAuthCredentialRenewal"/>
        /// </summary>
        public ReactorOAuthCredentialRenewal()
        {
            Clear();
        }

        /// <summary>
        /// Clears all values to default
        /// </summary>
        public void Clear()
        {
            m_ClientId.Clear();
            m_ClientSecret.Clear();
            m_TokenScope.Clear();
            m_ClientJwk.Clear();
        }

        /// <summary>
        /// Gets or sets the unique identifier that was used when sending the authorization request.
        /// </summary>
        public Buffer ClientId
        {
            get { return m_ClientId; }
            
            set { m_ClientId.Copy(value); }
        }

        /// <summary>
        /// Gets or sets the secret that was used by OAuth Client to authenticate with the token service. 
        /// </summary>
        public Buffer ClientSecret
        {
            get { return m_ClientSecret; }

            set { m_ClientSecret.Copy(value); }
        }

        /// <summary>
        /// Gets or sets the token scope that was used to limit the scope of generated token.
        /// </summary>
        public Buffer TokenScope
        {
            get { return m_TokenScope; }

            set { m_TokenScope.Copy(value); }
        }

        /// <summary>
        /// Gets or sets the JSON Web Key to authenticate with the token service.
        /// </summary>
        public Buffer ClientJwk
        {
            get { return m_ClientJwk; }

            set { ClientJwk.Copy(value); }
        }

        /// <summary>
        /// Performs a deep copy of <see cref="ReactorOAuthCredentialRenewal"/> object.
        /// </summary>
        /// <param name="destReactorOAuthCredentialRenewal">To copy OAuth credential renewal object into.
        /// It cannot be null.</param>
        /// <returns><see cref="ReactorReturnCode"/> value indicating success or failure of copy operations.</returns>
        public ReactorReturnCode Copy(ReactorOAuthCredentialRenewal destReactorOAuthCredentialRenewal)
        {
            Debug.Assert(destReactorOAuthCredentialRenewal is not null, "destReactorOAuthCredentialRenewal must be non-null");

            if (destReactorOAuthCredentialRenewal is null)
                return ReactorReturnCode.FAILURE;

            if(m_ClientId.Length != 0)
            {
                ByteBuffer byteBuffer = new ByteBuffer(m_ClientId.Length);
                m_ClientId.Copy(byteBuffer);
                byteBuffer.Flip();
                destReactorOAuthCredentialRenewal.ClientId.Data(byteBuffer);
            }

            if (m_ClientSecret.Length != 0)
            {
                ByteBuffer byteBuffer = new ByteBuffer(m_ClientSecret.Length);
                m_ClientSecret.Copy(byteBuffer);
                byteBuffer.Flip();
                destReactorOAuthCredentialRenewal.ClientSecret.Data(byteBuffer);
            }

            if (m_TokenScope.Length != 0)
            {
                ByteBuffer byteBuffer = new ByteBuffer(m_TokenScope.Length);
                m_TokenScope.Copy(byteBuffer);
                byteBuffer.Flip();
                destReactorOAuthCredentialRenewal.TokenScope.Data(byteBuffer);
            }

            if (m_ClientJwk.Length != 0)
            {
                ByteBuffer byteBuffer = new ByteBuffer(m_ClientJwk.Length);
                m_ClientJwk.Copy(byteBuffer);
                byteBuffer.Flip();
                destReactorOAuthCredentialRenewal.ClientJwk.Data(byteBuffer);
            }

            return ReactorReturnCode.SUCCESS;
        }
    }
}